#include "UI/Widget/HASInventoryWidget.h"
#include "Actor/HASItem.h"

FString UHASInventoryWidget::GetInformationDescription(const FItemStruct& ItemStruct)
{
	FDataTableRowHandle ItemHandle = ItemStruct.ItemHandle;
	if (FItemInfo* Info = ItemHandle.DataTable->FindRow<FItemInfo>(ItemHandle.RowName, ""))
	{
		if (ItemStruct.Rarity == EItemRarity::EIR_Common)
		{
			return FString::Printf(
				TEXT(
					"<Title> %s </> \n\n"

					"<Item> Rarity : </> <Rarity> %s </> \n\n"

					"<Item> Price : </> <Price> %d </> \n\n"
				),
				*Info->Name.ToString(),
				*GetRarityAsString(ItemStruct.Rarity),
				Info->Price
			);
		}
		else if (ItemStruct.Rarity == EItemRarity::EIR_Rare)
		{
			return FString::Printf(
				TEXT(
					"<Rare> %s </> \n\n"

					"<Item> Rarity : </> <Rare> %s </> \n\n"

					"<Item> Price : </> <Price> %d </> \n\n"

				),
				*Info->Name.ToString(),
				*GetRarityAsString(ItemStruct.Rarity),
				Info->Price * 2
			);
		}
		else if (ItemStruct.Rarity == EItemRarity::EIR_Unique)
		{
			return FString::Printf(
				TEXT(
					"<Unique> %s </> \n\n"

					"<Item> Rarity : </> <Unique> %s </> \n\n"

					"<Item> Price : </> <Price> %d </> \n\n"

				),
				*Info->Name.ToString(),
				*GetRarityAsString(ItemStruct.Rarity),
				Info->Price * 3
			);
		}
		else
		{
			return FString::Printf(
				TEXT(
					"<Legendary> %s </> \n\n"

					"<Item> Rarity : </><Legendary>%s</> \n\n"

					"<Item> Price : </> <Price> %d </> \n\n"

				),
				*Info->Name.ToString(),
				*GetRarityAsString(ItemStruct.Rarity),
				Info->Price * 4
			);
		}
	}

	return FString();
}

FString UHASInventoryWidget::GetRarityAsString(EItemRarity Rarity)
{
	switch (Rarity)
	{
		case EItemRarity::EIR_Common:
			return FString("Common");
		case EItemRarity::EIR_Rare:
			return FString("Rare");
		case EItemRarity::EIR_Unique:
			return FString("Unique");
		case EItemRarity::EIR_Legendary:
			return FString("Legendary");
	}

	return FString();
}

FString UHASInventoryWidget::LineBreak(FString Text)
{
	return Text.Replace(L"/n", L"\n");
}
