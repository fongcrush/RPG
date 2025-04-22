// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG/RPG.h"
#include "ObjectPoolComponent.generated.h"


USTRUCT(BlueprintType)
struct FPool
{
	GENERATED_BODY()

	FPool()	{}
	FPool(const FPool& Other)
	{
		if (this != &Other)
		{
			Num = Other.Num;
			Stack.Empty();
			Stack = Other.Stack;
		}
	}

	FPool& operator=(const FPool& Other)
	{
		if (this != &Other)
		{
			Num = Other.Num;
			Stack.Empty();
			Stack = Other.Stack;
		}
		return *this;
	}

	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int Num = 0;

	UPROPERTY()
	TArray<TObjectPtr<UObject>> Stack;
};

/**
 * 오브젝트 풀에 사용할 엑터는 AActor::Reset() 재정의 필요,
 */
UCLASS()
class RPG_API UObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UObjectPoolComponent();
protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:
	TWeakPtr<FPool> GetPoolWeakPtr(const TSubclassOf<UObject>& ObjectClass)
	{
		return PoolWeakPtrMap.FindOrAdd(ObjectClass, MakeShared<FPool>(PoolMap.FindOrAdd(ObjectClass)));
	}

	/** 오브젝트를 풀에서 할당. 없다면 새로 만든다. */
	template <typename T>
	T* Assign(const TSubclassOf<UObject>& ObjectClass)
	{
		if (ObjectClass == nullptr)
		{
			LOG_CALLINFO_ERROR("클래스 타입이 null 입니다.");
			return nullptr;
		}

		// 기존 오브젝트을 찾고, 남은 오브젝트가 없다면 할당 후 새로 생성(+5)
		FPool& Pool = PoolMap.FindOrAdd(ObjectClass);
		if (Pool.Stack.Num() == 0)
		{
			Pool.Num += 5;
			Pool.Stack.Reserve(Pool.Num);
			PoolWeakPtrMap.Add(ObjectClass, MakeShared<FPool>(Pool));

			// 엑터 기반 / 오브젝트 기반
			if (ObjectClass->IsChildOf(AActor::StaticClass()))
			{
				for (int i = 0; i < 5; i++)
				{
					// 엑터 생성, 비활성화 후 풀에 추가
					AActor* NewActor = GetWorld()->SpawnActor<AActor>(ObjectClass, SpawnParams);
					NewActor->SetActorHiddenInGame(true);
					NewActor->SetActorEnableCollision(false);
					NewActor->SetActorTickEnabled(false);
					Pool.Stack.Push(NewActor);
				}
			}
			else
			{
				for (int i = 0; i < 5; i++)
				{
					// 오브젝트 생성 후 풀에 추가
					UObject* Object = NewObject<UObject>(GetWorld(), ObjectClass);
					Pool.Stack.Push(Object);
				}
			}
		}

		return StaticCast<T*>(Pool.Stack.Pop());
	}

	/** 오브젝트를 풀에 복원 */
	void Restore(UObject* const& InObject)
	{
		if (InObject) return;

		const TSubclassOf<UObject>& ObjectClass = InObject->GetClass();
		PoolMap.FindOrAdd(ObjectClass).Stack.Emplace(InObject);

		if (AActor* InActor = Cast<AActor>(InObject))
		{
			InActor->SetActorHiddenInGame(true);
			InActor->SetActorEnableCollision(false);
			InActor->SetActorTickEnabled(false);
			InActor->Reset();
		}
	}

private:
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	//	VARIABLES & PROPERTIES
	//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TMap<TSubclassOf<UObject>, FPool> PoolMap;
	TMap<TSubclassOf<UObject>, TSharedPtr<FPool>> PoolWeakPtrMap;
	FActorSpawnParameters SpawnParams;
};
