// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GloryAttributeWidget.h"

#include "ToolBuilderUtil.h"
#include "GlorySystem/GloryManagerBase.h"
#include "Kismet/GameplayStatics.h"


void UGloryAttributeWidget::BindAttributes()
{
	const AGloryManagerBase* GloryManager = Cast<AGloryManagerBase, AActor>(*UGameplayStatics::GetActorOfClass(GetWorld(), AGloryManagerBase::StaticClass()));

	if (!GloryManager) return;

	UAbilitySystemComponent* ASC = GloryManager->GetAbilitySystemComponent();
	
	UGloryAttributeSet* attributeSet = GloryManager->GetGloryAttributeSet();

	//initialization
	GloryPercent = attributeSet->GetGloryAttribute().GetNumericValue(attributeSet) /
		attributeSet->GetMaxGloryAttribute().GetNumericValue(attributeSet);
	GloryText = FString::Format(TEXT("%d / %d"), {FMath::RoundToInt(attributeSet->GetGloryAttribute().GetNumericValue(attributeSet)),
		FMath::RoundToInt(attributeSet->GetMaxGloryAttribute().GetNumericValue(attributeSet))});

	ASC->GetGameplayAttributeValueChangeDelegate(attributeSet->GetGloryAttribute()).AddLambda([this, attributeSet](const FOnAttributeChangeData& Data)
	{
		GloryPercent = Data.NewValue / attributeSet->GetMaxGloryAttribute().GetNumericValue(attributeSet);
		GloryText = FString::Format(TEXT("{0} / {1}"), {FMath::RoundToInt(attributeSet->GetGloryAttribute().GetNumericValue(attributeSet)), FMath::RoundToInt(attributeSet->GetMaxGloryAttribute().GetNumericValue(attributeSet))});
	});
}
