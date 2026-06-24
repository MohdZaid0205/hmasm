# **HMASM**

![Banner](https://capsule-render.vercel.app/api?type=blur&height=300&color=0:3355ff,100:55ff55&text=[H]old%20[M]y%20Assembler&textBg=false&fontColor=ffffff&fontAlign=50&fontAlignY=50&reversal=false&section=header)

[![C](https://img.shields.io/badge/C-00599C?logo=c&logoColor=white)](#)
[![AssemblyScript](https://img.shields.io/badge/AssemblyScript-007AAC?logo=assemblyscript&logoColor=fff&color=555)](#)
[![CMake](https://img.shields.io/badge/CMake-000?logo=cmake&logoColor=white)](#)
[![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)](#)
[![Ubuntu](https://img.shields.io/badge/Ubuntu-E95420?logo=ubuntu&logoColor=white)](#)
[![Windows](https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white)](#)
[![macOS](https://img.shields.io/badge/macOS-000000?logo=apple&logoColor=F0F0F0)](#)

---

Hold My **Assembler** as it is a **modular**, **retargetable** assembler built to demystify the translation from mnemonic to machine code. Designed with a strict **3-Layer Architecture**, so that it completely decouples the parsing of assembly syntax from the generation of binary executables.

![overview-diagram](./resources//images/3-Layer-Architecture-Overview-Diagram.drawio.svg)


## **Installation** and **Usage**

**HMSAM** can only be installed through repository clone and compilation.
> Binaries and Installation procedure will be provided once `hmasm` development is complete.

```cmd
git clone https://github.com/MohdZaid0205/hmasm.git --recursive
```

### Install `build tools`

```cmd
winget install Kitware.CMake
winget install Ninja-build.Ninja
```

```bash
sudo apt install cmake
sudo apt install make
```

Verify build tools installation by.
```
cmake --version
ninja --version
```

if not installed properly please download installers from [cmake installation site](https://cmake.org/download/) and [ninja installation site](https://github.com/ninja-build/ninja/releases) and redo verification steps.

### Build `HMASM`

To build project hmasm with options `INSTALL_IN_SYSTEM` to install it in `bin` directory of your system. Otherwise binaries are added to `"{SOURCE_DIR}/bin"`. 

```cmd
cmake --list-presets
cmake --preset <preset-name> -DINSTALL_IN_SYSTEM=ON
```

> you will be given a list of `presets` to choose from, select one of them, for development purpose any preset with `-debug` will be beneficial, for usage purpose use one containing `-release`.

> if you do not want installation to occur at system-level do not pass flag `-DINSTALL_IN_SYSTEM`

after project has been built, you can build executables and install them.

```cmd
cmake --build . --list-presets
cmake --build . --preset <build-preset-name> --clean-first --target install
```

> `Administrator` privileges maybe required if installation target was  `system`.

> In windows you may need to add installation target to `PATH` for direct usage. 

### Use `HMASM`

To use `HMASM` use binaries contained within `bin/`.

```cmd
hmasm file.s --format bin --isa riscv --out out.bin
...
```

> usage is incomplete until project is in usable state

## Contributions

this project was built and planned solely by [Mohd Zaid](https://github.com/MohdZaid0205) (developer) while [Rishabh Gupta](https://github.com/RishabhGuptaGH) (contributer) did help with language [GRAMMAR](https://github.com/MohdZaid0205/hmasm/blame/8096afb26ca7538e3dd216526bbefeac18f0bcaf/resources/grammar.txt) which was later declared DEPRECATED. 
