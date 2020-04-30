# Development

---
- **4/19/2020** Copied Cinder-Stk block to repository
    - Couldn't manage to link it to rest of project

- **4/20/2020** Finished CMakeLists.txt for Cinder-Stk block to
                successfully build with the rest of the project

- **4/22/2020** Rewrote test app included with Cinder-Stk to test
                audio synthesis; test succeeded

- **4/23/2020** Fixed small bugs that caused crashing

- **4/26/2020** Implemented CSV file parser to begin integration of
                COVID-19 data into the application.
    - Had small issue with CMake that prevented tests running, but
      that issue was fixed with a single line in the CMakeLists.txt

- **4/27/2020** Implemented importing of data from COVID-19 data sources
                into application and wrote unit tests for implementation.

- **4/28/2020** Added UI for user to select dataset and region to sonify.
    - Data sonification did not work at this point - simply UI.

- **4/29/2020** Implemented data sonification after selecting dataset and
                region.
    - Included min/max MIDI pitches and scales as parameters for user
    - Allowed sonification playback to be controlled by master gain