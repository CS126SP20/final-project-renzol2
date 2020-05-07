# COVID-19 Data Sonification (CDS)
**Author**: Renzo Ledesma - [`renzol2@illinois.edu`](mailto:renzol2@illinois.edu)

[![license](https://img.shields.io/badge/license-MIT-green)](LICENSE)
[![docs](https://img.shields.io/badge/docs-yes-brightgreen)](docs/README.md)

![Visualization in CDS](https://i.imgur.com/lTjYK9O.png)
![Settings in CDS](https://i.imgur.com/zMm5urB.png)

COVID-19 Data Sonification (CDS) is a sonification/visualization
 tool for data about the COVID-19 global pandemic.
 
Audio synthesis is provided through the [Cinder-Stk](https://github.com/richardeakin/Cinder-Stk)
block, a wrapper for [STK (Synthesis ToolKit)](https://ccrma.stanford.edu/software/stk/index.html),
 a collection of open source audio signal processing classes written in C++. 

CDS's foundation is based off of 
Cinder-Stk's [test application](https://github.com/richardeakin/Cinder-Stk/blob/master/samples/StkTest/src/StkTestApp.cpp).

<h2>Dependencies</h2>

- [cmake](https://cmake.org/)
- [Visual Studio 2019](https://visualstudio.microsoft.com/visual-cpp-build-tools/)
- [Cinder](https://libcinder.org/)
- [Cinder-Stk](https://github.com/richardeakin/Cinder-Stk)

CDS requires a build of Cinder v.0.9.3, which can be found on their website.
This project repository should be built in a subdirectory of your local Cinder
repository.

In your Cinder repository, you should also have the Cinder-Stk block copied
as a sub-directory in the 'blocks' folder of your local Cinder repository.

Finally, you will need Visual Studio 2019 Build Tools installed.

<h2>Usage</h2>

| Key           | Action                       |
|---------------|------------------------------|
| `[` `]`       | Change instrument            |
| `o` `p`       | Change effect                |
| `t` `y`       | Change dataset               |
| `h` `g`       | Change region                |
| `x` `z`       | Change upper bound of data   |
| `Mouse/Click` | Play instrument with effect  |

All settings can be changed in CDS through the cursor.

<h2>Sources</h2>

COVID-19 (Coronavirus) data is courtesy of 
[Our World In Data](https://ourworldindata.org/coronavirus-source-data).


*Hannah Ritchie (2019) - "Coronavirus Source Data". 
Published online at OurWorldInData.org. Retrieved from: 'https://ourworldindata.org/coronavirus-source-data' [Online Resource]*

Synthesis ToolKit (STK) is written by Perry R. Cook & Gary P. Scavone.

Cinder-Stk is written by [Richard Eakin](https://rteakin.com/).
His Github can be found [here](https://github.com/richardeakin).