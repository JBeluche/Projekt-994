// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994/Game/Projekt994GameModeBase.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994ZombieSpawnPoint.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994PlayerSpawnPoint.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994GameState.h"
#include "Projekt994/Public/Player/CharacterBase.h"
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
            ZombieSpawnPoints.Add(SpawnPoint);
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Spawn point ZOMBIES count: %d"), ZombieSpawnPoints.Num());

    GetWorld()->GetTimerManager().SetTimer(TZombieSpawnHandle, this, &AProjekt994GameModeBase::SpawnZombie, .1f, true);
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
            if (APawn *Pawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, FRotator::ZeroRotator))
            {
                UE_LOG(LogTemp, Warning, TEXT("HERE I AM: "));
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
    UE_LOG(LogTemp, Warning, TEXT("Spawn point count: %d"), PlayerSpawnPoints.Num());
    bHasLoadedSpawnPoints = true;
}

void AProjekt994GameModeBase::SpawnZombie()
{
    if (ZombiesRemaining > 0)
    {
        int RandomIndex = FMath::RandRange(0, ZombieSpawnPoints.Num() - 1);

        if (AProjekt994ZombieSpawnPoint *SpawnPoint = ZombieSpawnPoints[RandomIndex])
        {

            FVector Loc = SpawnPoint->GetActorLocation();
            FRotator Rot = SpawnPoint->GetActorRotation();

            if (AZombieBase *Zombie = GetWorld()->SpawnActor<AZombieBase>(ZombieClass, Loc, Rot))
            {
                UE_LOG(LogTemp, Warning, TEXT("SPAWNING ZOMBIES BITCHES!"));
                --ZombiesRemaining;
            }
        }
    }
    else
    {
        GetWorld()->GetTimerManager().PauseTimer(TZombieSpawnHandle);
    }
}

void AProjekt994GameModeBase::CalculateZombieCount()
{
    if (ZombieGameState)
    {
        uint16 RoundNumber = ZombieGameState->GetRoundNumber();
        //Do calculation here
        ZombiesRemaining = 5;
    }
}
