# GCOZ

## About

GCOZ is a causal profiler for DirectX11 methods.
Causal profiling is a technique that, in short, simulates speeding up a method by slowing everything else down in order to find where optimization effort could yield the highest improvement.
This is called virtual speedup.
In order to reduce complexity, GCOZ only tries 10 different virtual speedups (10% steps from 0-1).
For more information you can [check out the original causal profiler](https://github.com/plasma-umass/coz) or read the [original paper](https://arxiv.org/pdf/1608.03676v1) by Curtsinger and Berger.
This project was developed as part of my bachelor's thesis at the university of Innsbruck in 2023 (which is also part of this repo).

The profiler is a commandline tool that works by injecting a .dll file into a running game using DirectX11.
This .dll then uses [rebzzel/kiero](https://github.com/Rebzzel/kiero) to hook into all possible methods in order to manipulate the rendering process.
It then walks through a number of predefined steps (each step is run for 500 frames), saves all collected data into the `data` directory and exits.
### Profiling steps
- Measure the duration of every method
- Collect information about which thread calls which method
- Profile the game's use of DirectX11
  
  The actual profiling consists once again of multiple steps
  - apply 0-90% delay to every method
  - virtually speed up a random method by a random amount

  Only methods with duration longer than one microsecond are used for profiling.
  This is due to my machine's timer having an accuracy of 100ns, and 10% of one microsecond is 100ns.

For more information please consult the pdf of my thesis found in the `miscellaneous` directory.

## Setup

The profiler uses boost, so please ensure that it is installed and point the solution to the correct location.

In order to inject the .dll, the profiler includes a hardcoded path to the .dll file, so ensure that this path is updated according to your location.

Now it should be possible to build the solution.
It is important to build it in DEBUG due to [assert statements used in kiero](https://github.com/Rebzzel/kiero/issues/41)

Most antivirus solutions block dll injection by default, so make sure you add the executable to exceptions.

There are also some settings that need to be changed on the host machine:
- `Windows Advanced Power Management`: this limits the maximum processor state of the machine, set this to ~10%
- VSync has to be turned off

For more information on these settings, please consult the thesis.

## Run

Open a Powershell window in the `gcoz_profiler/x64/Debug` directory.

Now run `.\gcoz_profiler <name of the game> <process id of the game>`

The profiler will now run through all the steps described above and write the data to the `data/` directory.

It also generates a new directory for every profiling run (e.g. `data/Borderlands 3/run_0`)

## Analysis
Open the `analysis.ipynb` jupyter notebook.

Replace value of the `_GAME` variable with the string you gave the profiler on invocation.

Replace the value of the `_RUN` variable with the specific folder in the `data/<game>` directory that you want to generate the visualizations for.

The notebook will generate a number of graphs (in the `graphs/` directory):
- AllFrameTimesBox.png: A number of graphs where each one shows every single frametime collected, ordered by the method that was delayed
- AllFrameTimesLine.png: Same as above, just in a line graph
- SingleFrametimesBox.png: Similar to `AllFrameTimesBox.png`, but showing the resulting frametimes when delaying all methods (serves a similar purpose to `BaselineAverage.png`)
- BaselineAverage.png: This is meant to be a 'sanity check'. This graph show the average of values in the [interquartile range](https://en.wikipedia.org/wiki/Interquartile_range) of the data. If the bars in this graph grow in a linear fashion, then the setup was completed correctly and the profiler can work as intended.
- resultAbs.png: The calculated, theoretical speedup in the time needed to render a single frame.
- resultRel.png: Same as above, just relative to the baseline.

**THIS PROJECT IS NO LONGER MAINTAINED AND ONLY SERVES AS AN ARCHIVE OF THE THESIS**