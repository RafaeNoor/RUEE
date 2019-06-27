# RUEE
RUEE uses LLVM-6.0 and CMake to build the Shared Object files. To use RUEE you would need to export environment variables:
`# RUEE
RUEE uses LLVM-6.0 and CMake to build the Shared Object files. To use RUEE you would need to export environment variables:


```export LLVM_COMPILER=clang   
export LLVM_CC_NAME=clang-6.0 
export LLVM_CXX_NAME=clang++-6.0  
export LLVM_LINK_NAME=llvm-link-6.0  
export LLVM_DIS_NAME=llvm-dis-6.0  
export LLVM_AR_NAME=llvm-ar-6.0  
export LLVM_AS_NAME=llvm-as-6.0  
export LLVM_LD_NAME=llvm-ld-6.0  
export LLVM_LLC_NAME=llc-6.0  
export LLVM_OPT_NAME=opt-6.0  
export LLVM_NM_NAME=llvm-nm-6.0  
export RUEE_HOME=/path/to/ruee/directory/  
```

A sample .env file has been provided and can be used by:  ` source ./.env`

## Build
To build the RUEE tool, export the above environment variables then:
```
mkdir build && cd build
cmake ../
make -j4
```

## Usage
Once the RUEE tool has been built, it can be used  via the python wrapper in `tool/ruee.py`  
`python3 tool/ruee.py {path/to/bc/file} {path/to/workdir/}`  

For example:  
`RUEE_HOME$: python3 tool/ruee.py test/testsA/testA.bc workdir/testA`  


RUEE is an attempt to implement the passes in CS701 http://pages.cs.wisc.edu/~fischer/cs701.f08/proj3.llvm.html. It is a work-in-progress .
