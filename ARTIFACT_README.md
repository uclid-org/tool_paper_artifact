# UCLID5 artifact


## Instructions for setup with docker

~~~
cat uclid_image.tar.gza* >uclid_image.tar.gz
docker run -i -t uclid_image.tar.gz /bin/bash
~~~

You are then inside the docker container

## Inside the docker container

The docker image mounts a folder containing the following directories and files:
- uclid_artefact/Readme.txt - a duplicate of this readme
- uclid_artefact/bin - precompiled binaries for UCLID5, Z3, CVC4, CVC5. The docker image automatically adds this to the path.
- uclid_artefact/compile_delphi.sh - a script to recompile Delphi for the docker container, and copy the binary into artefact/bin
- uclid_artefact/delphi - source code for delphi
- uclid_artefact/tool_paper_examples - scripts to run all of the examples in the tool paper
- uclid_artefact/uclid - the source code for UCLID5
- uclid_artefact/artifact.pdf - a PDF giving details of all the examples in the artifact


### Setup

First run the script `compile_delphi.sh`. This script recompiles Delphi and its dependencies for this docker container. 

The tool paper examples can then be reproduced using the prebuilt binaries.


### Running `UCLID5` on the examples:

 - We have provided a self-contained directory - `tool_paper_examples` - with all prebult binaries and examples. This does not require you to perform steps 1 and 2 above.
 - Please enter this directory and run the bash scripts contained in that folder. Each script corresponds to an example in the paper
 - For rudimentary testing purposes, we note that the script run_Fib.sh is the fastest script to run. 
 - refer to Section 2 of `artifact.pdf` for more details



### Availability statement:

The `UCLID5` tool is publicly available at https://github.com/uclid-org/uclid/tree/master. This repository includes the source code as well as a comprehensive set of tests.


### Badges:

We apply for all three badges: Functional, Reusable, Available.


### (Optional) Compiling UCLID5 from source:

Building `UCLID5` from source (needs an internet connection to install sbt):
   - enter the uclid directory
   - Follow the instructions in Readme.md

