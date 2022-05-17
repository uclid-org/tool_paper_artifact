# UCLID5 artifact setup


## Instructions for setup with docker

### Using the pre-saved docker image

~~~
docker load -i uclid_image.tar.gz
docker run -i -t uclid_image /bin/bash
~~~


## Inside the docker container

The docker image mounts a folder containing the following directories and files:
- uclid_artefact/Readme.txt - a readme with instructions on how to run the artifact
- uclid_artefact/bin - precompiled binaries for UCLID5, Z3, CVC4, CVC5, and
delphi. The docker image automatically adds this to the path.
- uclid_artefact/tool_paper_examples - scripts to run all of the examples in the tool paper
- uclid_artefact/uclid - the source code for UCLID5
- uclid_artefact/artifact.pdf - a PDF giving details of all the examples in the artifact

