
https://github.com/user-attachments/assets/afc76e2f-0b01-4427-8085-b326c03b9bd0

# Kinemotion

[![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.x-blue.svg)](https://www.unrealengine.com/)
[![Module](https://img.shields.io/badge/Plugin-Runtime-informational.svg)](#architecture)
[![Version](https://img.shields.io/badge/Version-1.0-orange.svg)](https://github.com/vicvasper/Kinemotion_UE5.5-/releases)

Monocular AI motion capture for Unreal Engine 5.x. Kinemotion ingests webcam video, runs inference with Unreal’s Neural Network Engine (NNE), and streams a 19-bone skeleton through Live Link for retargeting to any `USkeletalMeshComponent` in editor or PIE.

## Features
- **Direct Live Link source**: Creates and registers the Live Link source and subject (`SubjectName`) without external apps.
- **NNE inference**: Runs the pose model referenced by `PoseModel` on the runtime named by `NneRuntimeName` (DirectML GPU by default), decoding 133 keypoints into a 19-bone UE mannequin rig.
- **Automatic calibration**: Detects pelvis, neck, and the mesh bounding box to align scale and root; optional floor root (`bUseFloorAsRoot`). Re-runnable at any time via `Recalibrate()`.
- **Configurable scaling**: Isotropic scaling against either the measured skeleton height or an absolute reference (`StickmanScale`, `bScaleRelativeToSkeleton`, `ReferenceSizeCM`).
- **Anti-jitter filtering**: Exponential smoothing, dead zone, and step clamp (`SmoothingAlpha`, `DeadzoneCM`, `MaxStepCM`).
- **Debug overlays**: Draws spheres/lines for the stickman, bounding box, and key points in world space.

## Requirements
- Unreal Engine 5.3 or higher (modules: LiveLink, Media/MediaAssets, NNE, RHI/RenderCore).
- Engine plugins enabled: **LiveLink**, **LiveLinkAnimationCore**, **NNE**.
- A webcam recognized by `MediaPlayer` (`CameraIndex` selects the device).
- A project actor with a `USkeletalMeshComponent` that matches the standard mannequin bones (pelvis, spine_01, neck_01, head, clavicle/upperarm/lowerarm/hand, thigh/calf/foot).

## Installation
1) Copy `Plugins/Kinemotion` into your project (or clone the repo at the project root).
2) In the editor, open **Edit > Plugins** and enable **Kinemotion** (category: *Other*).
3) For C++ projects: right-click the `.uproject` → **Generate Visual Studio project files** → build the solution.
4) Restart the editor.

## Quick start
1) Add an Actor with a `USkeletalMeshComponent` (your target character) and attach the `KinemotionMocap` component.
2) Set key parameters:
   - `SubjectName`: Live Link subject (default `KinemotionWebcam`).
   - `CameraIndex`: webcam index (0 by default).
   - `bAutoCalibrate`: run calibration on begin play.
   - `bUseFloorAsRoot` / `bPelvisFree`: choose floor root or pelvis root; allow or lock pelvis translation.
   - `StickmanScale`: capture scale relative to the mesh.
   - `bEnableSmoothing`, `SmoothingAlpha`, `DeadzoneCM`, `MaxStepCM`: anti-jitter filter.
3) Press **Play/PIE**. The component:
   - Opens the webcam and reads frames via `MediaTexture`.
   - Preprocesses the image and runs NNE inference.
   - Decodes keypoints, calibrates, applies filters.
   - Publishes Live Link data (`SubjectName`) and renders debug (if enabled).
4) In **Window > Live Link**, verify the source "Kinemotion Mocap" and the subject `SubjectName`, then assign it for retargeting.

## Architecture
- **Runtime module** (`Kinemotion`): loads Live Link dependencies in `StartupModule`.
- **Component `UKinemotionMocap`**
  - `SetupLiveLinkDirect()`: creates `FKinemotionLiveLinkSource`, registers a 19-bone skeleton, and pushes static data.
  - `InitNNE()`: resolves the `PoseModel` soft reference, creates the model instance on `NneRuntimeName`, and sets the tensor shapes.
  - `InitMedia()` / `RequestTextureRead()`: opens the webcam via `MediaPlayer`, reads the texture on the render thread, and hands pixels back to the game thread. The read captures a weak pointer, so a component destroyed mid-flight cannot be written through.
  - `PreProcessImage()` / `RunInference()` / `DecodeAndPublish()`: prepares tensors, runs inference, applies calibration/filtering, and publishes Live Link frames.
  - `CalibrateFromSkeleton()` / `CalculateEffectiveScale()`: derives bounding box, selects floor or pelvis root, and computes isotropic scale.
  - `ApplySmoothing()`: EMA filter with dead zone and step clamp.

## Key parameters
- **Identity & input**: `SubjectName`, `CameraIndex`.
- **Model**: `PoseModel`, `NneRuntimeName`.
- **Scale**: `StickmanScale`, `bScaleRelativeToSkeleton`, `ReferenceSizeCM`.
- **Root & floor**: `bUseFloorAsRoot`, `FloorZOffset`, `bPelvisFree`.
- **Calibration**: `bAutoCalibrate`.
- **Rig**: `ClavicleBlend`, `YawCorrectionDegrees`, `LiveLinkFrameRate`.
- **Filters**: `bEnableSmoothing`, `SmoothingAlpha`, `DeadzoneCM`, `MaxStepCM`.
- **Debug**: `bShowDebug`, `DebugPointRadius`, `DebugBoneThickness`, `DebugDrawLifetime`.

> Per-axis scale (`ScaleX/Y/Z`), `GlobalScaleFactor` and `bDebugCalibration` were removed in
> the 2026 cleanup. They were editable but never read, so setting them did nothing.

## Troubleshooting
- **Live Link source not visible**: ensure Live Link plugins are enabled; `StartupModule` loads them. Check `SubjectName` collisions.
- **NNE model not loading**: confirm plugin content is enabled and that `PoseModel` points at a valid `UNNEModelData` asset. To use a different runtime, set `NneRuntimeName` in the details panel; no code change is needed.
- **Webcam not opening**: verify `CameraIndex` and that the device appears in `MediaBlueprintFunctionLibrary::EnumerateVideoCaptureDevices`.
- **Jitter or spikes**: raise `SmoothingAlpha`, tune `DeadzoneCM` / `MaxStepCM`, or lock pelvis (`bPelvisFree = false`).
- **Incorrect height/scale**: call `Recalibrate()`, then adjust `StickmanScale`, `ReferenceSizeCM` or `FloorZOffset`.
- **Capture faces the wrong way**: adjust `YawCorrectionDegrees` (default 90°).

## Live Link skeleton layout
Bone order (19): `root`, `pelvis`, `spine_01`, `neck_01`, `head`, `clavicle_l`, `upperarm_l`, `lowerarm_l`, `hand_l`, `clavicle_r`, `upperarm_r`, `lowerarm_r`, `hand_r`, `thigh_l`, `calf_l`, `foot_l`, `thigh_r`, `calf_r`, `foot_r`.

## Known limitations
- Beta state; not packaged for Marketplace yet.
- Focused on editor/PIE; verify NNE and Media behavior in cooked builds.
- Requires a mannequin-compatible skeleton for direct retargeting.

## Author
Created by Victor Rivas ([@vicvasper](https://github.com/vicvasper)).

## Links
- Repository: [github.com/vicvasper/Kinemotion_UE5.5-](https://github.com/vicvasper/Kinemotion_UE5.5-)
- Plugin folder: `Plugins/Kinemotion`
- Module: `Plugins/Kinemotion/Source/Kinemotion`
