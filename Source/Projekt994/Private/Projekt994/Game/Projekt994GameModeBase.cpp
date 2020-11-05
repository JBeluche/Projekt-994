// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994/Game/Projekt994GameModeBase.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994ZombieSpawnPoint.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994PlayerSpawnPoint.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994GameState.h"
#include "Projekt994/Public/Player/Projekt994Character.h"
#include "Projekt994/Public/Projekt994//Useables/Barricade.h"
#include "Projekt994/Public/Projekt994/Zombie/ZombieBase.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AProjekt994GameModeBase::AProjekt994GameModeBase()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BlueprintClasses/Player/BP_Projekt994Character.BP_Projekt994Character_C"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;
    bHasLoadedSpawnPoints = false;

    ZombiesRemaining = 0;
}

void AProjekt994GameModeBase::BeginPlay()
{
    Super::BeginPlay();

    ZombieGameState = GetGameState<AProjekt994GameState>();
    CalculateZombieCount();

    TArray<AActor *> TempActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProjekt994ZombieSpawnPoint::StaticClass(), TempActors);
    for (AActor *Actor : TempActors)
    {
        if (AProjekt994ZombieSpawnPoint *SpawnPoint = Cast<AProjekt994ZombieSpawnPoint>(Actor))
        {
            if (ABarricade *LinkedBarricade = SpawnPoint->GetLinkedBarricade())
            {
                SpawnPoint->SetZone(LinkedBarricade->GetAccessZone());
                ZombieSpawnPoints.Add(SpawnPoint);
            }
            else
            {
                ActiveZombieSpawnPoints.Add(SpawnPoint);
            }
        }
    }
    GetWorld()->GetTimerManager().SetTimer(TZombieSpawnHandle, this, &AProjekt994GameModeBase::SpawnZombie, 2.0f, true);
    GetWorld()->GetTimerManager().PauseTimer(TZombieSpawnHandle);
    CalculateZombieCount();
}

void AProjekt994GameModeBase::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (!bHasLoadedSpawnPoints)
    {
        SetSpawnPoints();
    }

    for (AProjekt994PlayerSpawnPoint *SpawnPoint : PlayerSpawnPoints)
    {
        if (!SpawnPoint->IsUsed())
        {
            FVector SpawnLocation = SpawnPoint->GetActorLocation();
            if (APawn *Pawn = GetWorld()->SpawnActor<APawn>(PlayerClass, SpawnLocation, FRotator::ZeroRotator))
            {
                NewPlayer->Possess(Pawn);
                SpawnPoint->SetUsed(true);
                return;
            }
        }
    }
}

void AProjekt994GameModeBase::SetSpawnPoints()
{
    TArray<AActor *> TempActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AProjekt994PlayerSpawnPoint::StaticClass(), TempActors);

    for (AActor *Actor : TempActors)
    {
        if (AProjekt994PlayerSpawnPoint *SpawnPoint = Cast<AProjekt994PlayerSpawnPoint>(Actor))
        {
            PlayerSpawnPoints.Add(SpawnPoint);
        }
    }
    bHasLoadedSpawnPoints = true;
}

void AProjekt994GameModeBase::SpawnZombie()
{
    if (ZombieGameState)
    {
       uint8 PlayerCount = ZombieGameState->PlayerArray.Num();
        uint8 MaxZombiesOnMapAtOnce = 24;
       if (PlayerCount > 1)
        {
            MaxZombiesOnMapAtOnce += PlayerCount * 6;
        }
        if (ZombiesRemaining > 0 && ZombieGameState->GetZombiesOnMap() <= MaxZombiesOnMapAtOnce - 1)
        {
            int RandomIndex = FMath::RandRange(0, ActiveZombieSpawnPoints.Num() - 1);

            if (AProjekt994ZombieSpawnPoint *SpawnPoint = ActiveZombieSpawnPoints[RandomIndex])
            {

                FVector Loc = SpawnPoint->GetActorLocation();
                FRotator Rot = SpawnPoint->GetActorRotation();

                if (AZombieBase *Zombie = GetWorld()->SpawnActor<AZombieBase>(ZombieClass, Loc, Rot))
                {
                    ZombieGameState->ZombieSpawned();
                    --ZombiesRemaining;
                }
            }
        }
        else if(ZombiesRemaining <= 0)
        {
           GetWorld()->GetTimerManager().PauseTimer(TZombieSpawnHandle);
        }
    }
}

void AProjekt994GameModeBase::NewZoneActive(uint8 ZoneNumber)
{
    int Control = 0;
    for (int16 x = ZombieSpawnPoints.Num() - 1; x >= 0; --x)
    {
        AProjekt994ZombieSpawnPoint *SpawnPoint = ZombieSpawnPoints[x];

        ++Control;

        if (SpawnPoint && ZoneNumber == SpawnPoint->GetZone() && !SpawnPoint->IsActive())
        {
            ActiveZombieSpawnPoints.Add(SpawnPoint);
            SpawnPoint->Activate();
            //Remove spawnpoiint from the zombiespawnpoint array
            ZombieSpawnPoints.RemoveAt(x);
        }
    }
}

void AProjekt994GameModeBase::ZombieKilled()
{
    if (ZombieGameState)
    {
        ZombieGameState->ZombieKilled();
        if (ZombieGameState->GetTotalZombiesRemaining() == 0)
        {
            //Start new round
            ZombieGameState->IncrementRoundNumber();
            FTimerHandle TempHandle;
            GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &AProjekt994GameModeBase::CalculateZombieCount, 10.0f, false);
        }
    }
}

void AProjekt994GameModeBase::CalculateZombieCount()
{
    if (ZombieGameState)
    {
        uint16 RoundNumber = ZombieGameState->GetRoundNumber();
        uint8 PlayerCount = ZombieGameState->PlayerArray.Num();
        uint8 MaxZombiesOnMapAtOnce = 24;
        if (PlayerCount > 1)
        {
            MaxZombiesOnMapAtOnce += (PlayerCount * 6);
        }
        UE_LOG(LogTemp, Warning, TEXT("PlayerCount: %d"), PlayerCount);

        if (RoundNumber > 0 && RoundNumber <= 5)
        {
            UE_LOG(LogTemp, Warning, TEXT("Rounds 1-5"));
            ZombiesRemaining = FMath::FloorToInt((RoundNumber * .2f) * MaxZombiesOnMapAtOnce);
            UE_LOG(LogTemp, Warning, TEXT("Zombies Remaining: %d"), ZombiesRemaining);
        }
        else
        {
            ZombiesRemaining = FMath::FloorToInt((RoundNumber * .15f) * MaxZombiesOnMapAtOnce);
        }

        ZombieGameState->SetTotalZombiesRemaining(ZombiesRemaining);
        GetWorld()->GetTimerManager().UnPauseTimer(TZombieSpawnHandle);
    }
}