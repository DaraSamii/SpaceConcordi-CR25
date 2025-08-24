# Supersonic Rocket CFD Analysis

A comprehensive CFD study of a model rocket in supersonic flow using OpenFOAM and cfMesh. This repository contains simulation setup, analysis tools, and results for aerodynamic coefficients, flow visualization, and structural loads across various Mach numbers and angles of attack.

# Abstract 
Aether was SpaceConcordia's non-ordinary transition rocket launched on Monday, August 18th, 2025, in Timmins, Ontario, for Launch Canada. Due to its novel design and the team's limited experience with this configuration, Computational Fluid Dynamics (CFD) was utilized to provide the design team with better understanding of the forces and aerodynamic loads acting on the rocket. We hope that computer-aided techniques such as CFD become more common in student teams, and we have demonstrated that it can be a powerful tool to assess aerodynamic forces and flight performance during different stages of flight. Launch Canada safety officials generally do not accept undergraduate CFD simulations, believing that without proper knowledge, CFD is merely "CAD in, fancy contours out." This report serves as a guide to help future practitioners perform proper mesh analysis, select appropriate solvers and boundary conditions, and configure cases to obtain verified, stable, and valid outputs that meaningfully assist other team members in designing better rockets.
I have endeavored to make this report as detailed as possible because the intended audience consists primarily of 2nd or 3rd-year undergraduates with limited experience in CFD and specifically OpenFOAM. Therefore, the first chapters describe the governing physics and theory behind each decision. In the appendix, actual OpenFOAM codes and a guide for submitting jobs to HPC systems are provided. Recognizing that undergraduate mechanical engineering students typically lack Linux experience, one full chapter is dedicated to teaching basic Linux bash commands.
It is the author's hope that readers will develop a passion for CFD and, upon discovering new methods and techniques to improve this report or the CFD codes, will document these improvements and pass them on to future generations.


# Repository Structure
```
OpenFOAM_Case/
├── cfmesh/
├── original/
│   ├── 0.orig/
│   │   ├── alphat
│   │   ├── k
│   │   ├── nut
│   │   ├── omega
│   │   ├── p
│   │   ├── T
│   │   └── U
│   ├── constant/
│   │   ├── extendedFeatureEdgeMesh/
│   │   ├── polyMesh/
│   │   ├── thermophysicalProperties
│   │   ├── triSurface/
│   │   └── turbulenceProperties
│   ├── system/
│   │   ├── blockMeshDict
│   │   ├── controlDict
│   │   ├── decomposeParDict
│   │   ├── fvOptions
│   │   ├── fvSchemes
│   │   ├── fvSchemes.accurate
│   │   ├── fvSchemes.stable
│   │   ├── fvSolution
│   │   ├── meshDict
│   │   └── surfaceFeatureExtractDict
│   ├── Allrun.sh
│   ├── foam.foam
│   ├── logs/
│   ├── Mesh.sh
│   ├── parameters.cs
│   ├── Pserver.sh
│   └── submit.sh
├── Data/
├── SubmitAll.py
└── analysis.ipynb
```

# Simulation Setup

# Flow Conditions
- **Mach Range**: 0.8 - 2.0
- **Angle of Attack**: 0° - 20°
- **Solver**: rhoSimpleFoam (compressible RANS)
- **Turbulence**: k-ω SST model
- **Mesh**: 25M cells with 7 inflation layers

# Governing Equations

**Conservation of Mass:**


$$\frac{\partial \rho}{\partial t} + \nabla \cdot (\rho \vec{U}) = 0$$

**Conservation of Momentum:**


$$\frac{\partial (\rho \vec{U})}{\partial t} + \nabla \cdot (\rho \vec{U} \otimes \vec{U}) = -\nabla p + \nabla \cdot \boldsymbol{\tau}_{\text{eff}} + \rho \vec{g}$$

**Conservation of Energy:**


$$\frac{\partial (\rho h)}{\partial t} + \nabla \cdot (\rho \vec{U} h) = \frac{Dp}{Dt} + \nabla \cdot \left( \frac{\mu + \mu_t}{Pr_t} \nabla h \right) + \boldsymbol{\tau}_{\text{eff}} : \nabla \vec{U}$$

**Equation of State:**


$$p = \rho R T = \frac{\rho R_u T}{M}$$

**Sutherland's Law:**


$$\mu(T) = A_s \frac{T^{3/2}}{T + T_s}$$

**k-ω SST Turbulence Model:**

Turbulent Kinetic Energy:


$$\frac{\partial (\rho k)}{\partial t} + \nabla \cdot (\rho \vec{U} k) = \nabla \cdot \left[ (\mu + \mu_t \sigma_k) \nabla k \right] + P_k - \beta^* \rho \omega k$$

Specific Dissipation Rate:


$$\frac{\partial (\rho \omega)}{\partial t} + \nabla \cdot (\rho \vec{U} \omega) = \nabla \cdot \left[ (\mu + \mu_t \sigma_\omega) \nabla \omega \right] + \frac{\lambda}{\nu_t} P_k - \beta \rho \omega^2 + 2(1 - F_1)\frac{\rho \sigma_{\omega 2}}{\omega} \nabla k \cdot \nabla \omega$$

Where:
- $P_k = \tau_{ij} \frac{\partial U_i}{\partial x_j}$ (production term)
- $F_1 = \tanh \left( \left(\min\left[ \max\left( \frac{\sqrt{k}}{\beta^* \omega d}, \frac{500 \nu}{d^2 \omega} \right), \frac{4 \rho \sigma_{\omega 2} k}{CD_{k\omega} d^2} \right]\right)^4 \right)$ (blending function)

## Air Properties

| Property | Value | Units |
|----------|-------|-------|
| Molar mass (M) | 28.9 | g/mol |
| Specific heat (C_p) | 1005 | J/(kg·K) |
| Sutherland constant (A_s) | 1.4792×10⁻⁶ | kg/(m·s·K^0.5) |
| Sutherland temperature (T_s) | 116 | K |
| Prandtl number (Pr) | 0.7 | - |
| Specific gas constant (R) | 287.0 | J/(kg·K) |

# Mesh
cfMesh was utilised to create the mesh


# Results

## Verification

## Validation

## Contours

## Force Coefficients

## Force loads



