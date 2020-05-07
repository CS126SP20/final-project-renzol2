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
    
- **5/5/2020** Added final changes in preparation for submission
    - Added total of 8 new datasets for a final total of 12 datasets available
      for sonification playback
        - Total cases per million
        - Total deaths per million
        - Daily increase of cases per million
        - Daily increase of deaths per million
        - Total tests
        - Daily change in tests
        - Total tests per thousand
        - Daily change in tests per thousand
    - Visualization can now be scaled by a percentage of screen height/width
    - Visualization graph can now be colored by user using RGBA values
    - Fixed bug where pitches lower/higher than set min/max were possible by 
      moving cursor out of the application window
    - Different upper bounds were added, which affect sonification and visualization
      - Regional maximum (maximum pitch mapped to highest point in current regional data)
      - International maximum (max pitch mapped to highest point in all regional data)
      - Cumulative maximum (max pitch mapped to highest cumulative point)
      
- **5/6/2020** Finalized documentation and comments across all source files. Also 
               wrote out DEVELOPMENT.md, README.md, and PROPOSAL.md. 
    
