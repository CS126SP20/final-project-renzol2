# COVID-19 Data Sonification

Author: Renzo Ledesma

### Project Goal
The goal of this project is to create sound using public data about the 
COVID-19 global pandemic, a process known as data sonification. By mapping 
certain points of data to certain musical/audio elements, data can be "heard" 
and conveyed through sound. This can have analytical, scientific use as we tend
to perceive audible information differently than visual information, allowing
scientists to find different patterns they otherwise couldn't.

### Background Knowledge
My major is CS + Music, and I have musical experience using DAWs (Digital Audio 
Workstations) and producing commercial-style music for my high school productions
team. I have experience using synthesizers and a basic knowledge of
how synthesizers work, which will aid me in this project.
Because of my major, I also have college-level music classes in theory,
ear training, and performance, alongside over a decade studying the piano as my
primary instrument.

My CS background goes back to junior year of high school, starting with Java
and C# and developing those skills until now in CS126 with C++. As part of the CS+Music
track, I have taken a Python lab geared towards using Python as a tool to
identify and correct issues in musical counterpoint, a study of music composition
geared towards the writing in multiple voices (common in the Baroque/Classical
era, though used extensively throughout musical history and even today.) 

With this background and the following libraries, I feel confident in my ability
to complete this project or at least learn a significant amount throughout the
process.

### External Libraries
On top of C++, I plan to use the audio synthesis library Synthesis ToolKit to
aid in the synthesizing aspect of this project.

https://github.com/richardeakin/Cinder-Stk

The above library is a wrapper for STK for use in Cinder, which should aid in
the use of STK in this particular project. With Cinder capabilities, I hope to
simultaneously visualize the data while also synthesizing audio based off of the
data.

Throughout the project, I will continue to do research on other audio synthesis 
projects and libraries that are compatible with Cinder.

For the data itself, I will use COVID-19 data provided by the Our World In Data,
which hosts public datasets that are updated every day.

### Musical Aspect of Data Sonification

For mapping musical/sonic elements to specific points or trends in the data, I am
still unsure about my procedure. However, I have several digital music/audio tools
at my disposal that I am comfortable using, and I am confident they will aid me
in the more artistic, musical aspect of this project.

Additionally, there are a few guidelines that have been used in practice; for example,
University of Minnesota undergraduate Daniel Crawford and geography professor
Scott St. George used global temperature data to compose a string quartet piece,
representing the different latitudinal zones through the different instruments
and the temperature data through the pitch of the notes played by those instruments.
By increasing the pitch of notes played in a specific instrument, the listener
can literally *hear* the increase in temperature of a specific region on the globe.

The video presentation on this composition can be found here: https://vimeo.com/127083533

This approach requires explanation before performance, obviously, and the "data"
itself is obviously not as clear as having labelled graphs; however, I do think
that data sonification offers listeners a more intuitive or even emotional 
approach to understanding trends in data. For example, a sudden rise in volume
or pitch in a musical piece could convey the exponential rise of infected
cases of COVID-19. Since this musical device is used to create drama, excitement,
tension, etc. in musical practice, those same emotions can be applied to the data.
This, I believe, is a merit of data sonification, and also a guideline to approach
this project musically.

POST-COMPLETION NOTE: After doing more research on data sonification as a topic, I
decided to keep the "musical" aspect of the project at a bare minimum. Sonification is,
at its core, a tool to interpret raw data, and forcing musical aesthetics would
not be beneficial in this case. The COVID-19 pandemic is an ongoing crisis, and
I wanted to keep this tool as unbiased and flexible as possible for anyone using it
without forcing my own musical or aesthetic preferences on it.

### Timeline
* Week 1: Successfully use the audio synthesis library to create sound, unrelated
to the data. This week should be committed towards verifying that the STK library
works within Cinder. If possible, I would also hope to input the data from the
NYTimes into this project, either through an API or just as local files on my
computer.
* Week 2: Devise a concrete plan to map aspects of the data to musical elements,
hopefully with a proof of concept through my DAWs.
* Week 3: Complete the application. It should visualize the data while procedurally
generating music in the background. Ideally, an element of randomness would
allow different compositions for any given viewing of the data.

---

