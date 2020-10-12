// Copyright by Creating Mountains

#include "Projekt994/Public/Projekt994/Game/Projekt994GameModeBase.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994ZombieSpawnPoint.h"
#include "Projekt994/Public/Projekt994/Game/Projekt994PlayerSpawnPoint.h"
#include "Projekt994/Public/Player/CharacterBase.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AProjekt994GameModeBase::AProjekt994GameModeBase()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/BlueprintClasses/Player/BP_CharacterBase.BP_CharacterBase_C"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;
    bHasLoadedSpawnPoints = false;

    // use our custom HUD class
}

void AProjekt994GameModeBase::BeginPlay()
{
    Super::BeginPlay();

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