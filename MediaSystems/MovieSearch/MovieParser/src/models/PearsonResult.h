#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file PearsonResult.h
 * @date 2025-09-25
 */

struct PearsonResult {
    double correlation{ 0.0 };
    int overlap{ 0 };
    bool valid{ false };
};