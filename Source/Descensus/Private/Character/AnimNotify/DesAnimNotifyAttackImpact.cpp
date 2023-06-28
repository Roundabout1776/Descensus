#include "Character/AnimNotify/DesAnimNotifyAttackImpact.h"

#include "DesLogging.h"

void UDesAnimNotifyAttackImpact::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	DES_LOG_CSTR(Warning, "Hit!!")
}
