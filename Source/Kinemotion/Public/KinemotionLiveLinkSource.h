// Copyright (c) Victor Rivas Perez. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ILiveLinkSource.h"

/**
 * Live Link source representing a Kinemotion capture component.
 *
 * Registering a source is what makes the subject visible and manageable in the Live Link
 * panel; pushing subject data without one leaves an entry the user cannot see or shut down.
 * The source itself carries no data - frames are pushed straight to the client by the
 * component - so its only real job is to exist and report status.
 */
class FKinemotionLiveLinkSource : public ILiveLinkSource
{
public:
	explicit FKinemotionLiveLinkSource(const FText& InSourceType)
		: SourceType(InSourceType)
	{
	}

	virtual ~FKinemotionLiveLinkSource() override = default;

	//~ Begin ILiveLinkSource
	virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) override
	{
		Client = InClient;
		SourceGuid = InSourceGuid;
	}

	/** The component owns this source's lifetime, so it stays valid until explicitly removed. */
	virtual bool IsSourceStillValid() const override { return true; }

	virtual bool RequestSourceShutdown() override
	{
		Client = nullptr;
		return true;
	}

	virtual FText GetSourceType() const override { return SourceType; }

	virtual FText GetSourceMachineName() const override
	{
		return FText::FromString(FPlatformProcess::ComputerName());
	}

	virtual FText GetSourceStatus() const override
	{
		return Client
			? NSLOCTEXT("Kinemotion", "SourceStatusActive", "Active")
			: NSLOCTEXT("Kinemotion", "SourceStatusDisconnected", "Disconnected");
	}
	//~ End ILiveLinkSource

private:
	FText SourceType;

	/** Set by the client on registration; cleared on shutdown. Drives the reported status. */
	ILiveLinkClient* Client = nullptr;

	FGuid SourceGuid;
};
