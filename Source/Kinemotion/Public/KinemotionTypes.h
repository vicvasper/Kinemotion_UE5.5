// KinemotionTypes.h
#pragma once

#include "CoreMinimal.h"

// Índices típicos de modelos tipo COCO/RTMW3D (Simplificado a 17 puntos base)
namespace KineKP
{
    enum Index
    {
        Nose = 0,
        LEye = 1, REye = 2,
        LEar = 3, REar = 4,
        LShoulder = 5, RShoulder = 6,
        LElbow = 7, RElbow = 8,
        LWrist = 9, RWrist = 10,
        LHip = 11, RHip = 12,
        LKnee = 13, RKnee = 14,
        LAnkle = 15, RAnkle = 16,
        Total = 17
    };
}

// Nombres de huesos para el esqueleto virtual que enviaremos a LiveLink
namespace KineBones
{
    static const FName Root = "Root";
    static const FName Hips = "Hips";
    static const FName Head = "Head";
    
    static const FName LeftArm = "LeftArm";
    static const FName LeftForeArm = "LeftForeArm";
    static const FName LeftHand = "LeftHand";
    
    static const FName RightArm = "RightArm";
    static const FName RightForeArm = "RightForeArm";
    static const FName RightHand = "RightHand";
    
    static const FName LeftUpLeg = "LeftUpLeg";
    static const FName LeftLeg = "LeftLeg";
    static const FName LeftFoot = "LeftFoot";
    
    static const FName RightUpLeg = "RightUpLeg";
    static const FName RightLeg = "RightLeg";
    static const FName RightFoot = "RightFoot";
}