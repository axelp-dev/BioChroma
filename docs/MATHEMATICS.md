# Some Mathematics 

> This file presents and develops all the necessary mathematics to understand the project. 

## Table of Contents
- [Some Mathematics](#some-mathematics)
  - [Table of Contents](#table-of-contents)
  - [1. Diffusion on the nutrient grid](#1-diffusion-on-the-nutrient-grid)
    - [1.1 The global formulae](#11-the-global-formulae)
    - [1.2 Solving it : the Euler method](#12-solving-it--the-euler-method)

---

## 1. Diffusion on the nutrient grid

**The core idea.** As implemented in the `include/Grid.h` and `src/Grid.cpp`, the grid is represented as a simple 2d array stored in a 1d `float` one. To perform this, we use a simple formulae which allows us to cast any 2d coordinate into a 1d one : 
$$ (x,y) \in N \times N \iff i = y \times N + x \in [0, N \times N]. $$

### 1.1 The global formulae 

**Diffusion.** At each step, we perform a *diffusion* operation on the grid: each discrete `Grid` cell share a bit of its nutrients to the near ones. To perform that, we use the Fick's laws. 

**Fick's Laws.** Those physics laws describes the diffusion process of particles in discrete fields: it's perfect in our case. Here is the general formulae: 

$$ 
\frac{\partial u}{\partial t} = D \nabla^{2} u = D \cdot \left( \frac{\partial^2 u}{\partial x^2} + \frac{\partial^2 u}{\partial y^2}\right). 
$$

with $D$ the *diffusion coefficient* which describes the amount of nutrients that the local cell $u$ share with the others. 

### 1.2 Solving it : the Euler method

**Laplacian.** In our case, we are on a discrete `N` times `N` grid of `cell_size` times `cell_size` cells. We approximate the above general formulae with a simple Laplacian 5-points tencil: 

$$ 
\nabla^2 u_{i,j} \simeq \frac{
    u_{i+1,j} + u_{i-1, j} + u_{i, j-1} + u_{i, j+1} - 4 u_{i,j}
}{\nabla x^2}. 
$$

**Euler.** Next a simple Euler explicit writing allows us to process it on a computer : 

$$ 
u_{i,j}^{t+1} = u_{i,j}^t + \Delta t \cdot D \cdot \left( \frac{u_{i+1,j} + u_{i-1, j} + u_{i, j-1} + u_{i, j+1} - 4 u_{i,j}
}{\nabla x^2}
\right). 
$$

Let $ \alpha = \frac{D \cdot \Delta t}{\nabla x^2}$, then we have : 

$$
u_{i,j}^{t+1} = u_{i,j}^t + \alpha \left( u_{neighbours}^t - 4 u_{i,j}^t \right). 
$$

**The $\alpha$.** This coefficient represents the amount of nutriments the current cell give to the others aroud it. To prevent divergence, we need to have $ \alpha \leqslant 0.25$. To make it more comprehensive for the user, we asked him to give a `diffusion_coeff` $d \in [0,1]$ at the initilization which will represent the same thing that $\alpha$. Then, we just init an `alpha` parameter using the identity: 
$$ \alpha = 0.25 \times d. $$