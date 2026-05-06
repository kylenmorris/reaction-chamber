# Reaction Chamber 

## Name
Reaction Chamber for Pathogen Detection in Crops

## Getting Started

### Prerequisites
* Raspberry Pi Pico SDK
  ```sh
  installation here
  ```
* CMake
  ```sh
  installation here
  ```

### Compilation and Flashing
While this project does make use of the Raspberry Pi Pico extension, there can be issues using it when first cloning the project. We are using CMake and Ninja for compilation, as is default for the Pico.

After pulling, navigate into the .\code\ directory
```
cd .\code
```
Create the build folder
```
New-Item -ItemType Directory -Name build ; cd build
```
Run CMake with Ninja, and build the project
```
cmake -G Ninja ..
ninja
```

### Running Tests



## Usage
Use examples liberally, and show the expected output if you can. It's helpful to have inline the smallest example of usage that you can demonstrate, while providing links to more sophisticated examples if they are too long to reasonably include in the README.

## Roadmap
If you have ideas for releases in the future, it is a good idea to list them in the README.

You can also document commands to lint the code or run tests. These steps help to ensure high code quality and reduce the likelihood that the changes inadvertently break something. Having instructions for running tests is especially helpful if it requires external setup, such as starting a Selenium server for testing in a browser.

## Authors and acknowledgment
Show your appreciation to those who have contributed to the project.

## License
For open source projects, say how it is licensed.

## Project status
If you have run out of energy or time for your project, put a note at the top of the README saying that development has slowed down or stopped completely. Someone may choose to fork your project or volunteer to step in as a maintainer or owner, allowing your project to keep going. You can also make an explicit request for maintainers.
