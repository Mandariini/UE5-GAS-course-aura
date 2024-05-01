// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeInfo.h"

#include "GameplayTagsManager.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
  for (const auto& Info : AttributeInfos)
  {
    if (Info.AttributeTag == AttributeTag)
    {
      return Info;
    }
  }

  if (bLogNotFound)
  {
    UE_LOG(LogTemp, Error, TEXT("Cannot find Info for AttributeTag [%s] on AttributeInfos [%s]"),
           *AttributeTag.ToString(), *GetNameSafe(this));
  }

  return FAuraAttributeInfo();
}
