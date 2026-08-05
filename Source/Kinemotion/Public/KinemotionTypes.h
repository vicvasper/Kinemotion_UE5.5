// Copyright (c) Victor Rivas Perez. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Fixed characteristics of the pose model shipped with the plugin.
 *
 * These are deliberately compile-time constants rather than editable properties: they are
 * dictated by the .onnx graph itself. Exposing them would let a user set values the model
 * cannot accept, and the failure would only surface as a shape-mismatch at inference time.
 * Swapping in a model with a different topology means changing these together.
 */
namespace KinemotionModel
{
	/** Network input resolution, in pixels. Camera frames are resampled to this. */
	constexpr int32 InputWidth = 288;
	constexpr int32 InputHeight = 384;

	/** Planar RGB. */
	constexpr int32 InputChannels = 3;

	/** Elements in one input tensor. */
	constexpr int32 InputElementCount = InputWidth * InputHeight * InputChannels;

	/** Whole-body keypoints predicted per frame; only the body subset below is consumed. */
	constexpr int32 JointCount = 133;

	/**
	 * SimCC classification bins per axis. The model predicts a distribution over bins rather
	 * than a coordinate, so decoding a joint means taking the argmax bin and normalising it.
	 * The axes carry different resolutions, which is why these are three separate values.
	 */
	constexpr int32 SimccBinsX = 576;
	constexpr int32 SimccBinsY = 768;
	constexpr int32 SimccBinsZ = 576;

	constexpr int32 OutputElementCountX = JointCount * SimccBinsX;
	constexpr int32 OutputElementCountY = JointCount * SimccBinsY;
	constexpr int32 OutputElementCountZ = JointCount * SimccBinsZ;

	/** Maximum value of an 8-bit colour channel, used to normalise pixels into [0, 1]. */
	constexpr float ColorChannelMax = 255.0f;
}

/**
 * Indices of the body keypoints inside the model's output, following the COCO ordering.
 *
 * The model emits KinemotionModel::JointCount keypoints covering face, hands and feet; only
 * the thirteen listed here drive the rig. Face and hand indices are intentionally absent
 * rather than commented out - the rig has no bones for them.
 */
namespace KinemotionKeypoint
{
	constexpr int32 Nose = 0;

	constexpr int32 LeftShoulder = 5;
	constexpr int32 RightShoulder = 6;
	constexpr int32 LeftElbow = 7;
	constexpr int32 RightElbow = 8;
	constexpr int32 LeftWrist = 9;
	constexpr int32 RightWrist = 10;

	constexpr int32 LeftHip = 11;
	constexpr int32 RightHip = 12;
	constexpr int32 LeftKnee = 13;
	constexpr int32 RightKnee = 14;
	constexpr int32 LeftAnkle = 15;
	constexpr int32 RightAnkle = 16;
}
