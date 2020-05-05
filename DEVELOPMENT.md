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

- **5/2/2020** Added BPM as a parameter during sonification playback.
    - Also added two new datasets (new cases/new deaths per day)

- **5/3/2020** Revamped visualization and appearance of app.
    - Added displays for dataset and note values (both data and pitch)
    - Cleaned parameters to only usable ones
    - Added directions in middle of screen
    - Partitioned parameters using separators
    - Implemented graph visualization during sonification playback

- **5/4/2020** Slightly changed behavior of visualization and settings
    - Added a visualization toggle switch (can now turn visualization off)
    - Visualization now stays after playback until user clicks anywhere
    - Settings are now hidden during visualization
    - Regions list in settings is now displayed in descending order by max amount
