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

This application's foundation is based off of 
Cinder-Stk's [test application](https://github.com/richardeakin/Cinder-Stk/blob/master/samples/StkTest/src/StkTestApp.cpp).

<h2>Dependencies</h2>
- [cmake](https://cmake.org/)
- [Visual Studio 2019](https://visualstudio.microsoft.com/visual-cpp-build-tools/)
- [Cinder-Stk](https://github.com/richardeakin/Cinder-Stk)

<h2>Usage</h2>
| Key           | Action                       |
|---------------|------------------------------|
| `[` `]`       | Change instrument            |
| `o` `p`       | Change effect                |
| `t` `y`       | Change dataset               |
| `h` `g`       | Change region                |
| `x` `z`       | Change upper bound of data   |
| `Mouse/Click` | Play instrument with effect  |

All settings can be changed in-application through the cursor.

<h2>Data Sources</h2>
COVID-19 (Coronavirus) data is courtesy of 
[Our World In Data](https://ourworldindata.org/coronavirus-source-data)
---
*Hannah Ritchie (2019) - "Coronavirus Source Data". Published online at OurWorldInData.org. Retrieved from: 'https://ourworldindata.org/coronavirus-source-data' [Online Resource]*

