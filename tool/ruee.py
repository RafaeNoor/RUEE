import sys
import subprocess
import os

#python3 driver.py input.bc workdir

input_file = sys.argv[1].split("/")[-1]
input_file_name = input_file.split(".")[0]

work_dir = sys.argv[2]
RUEE_HOME = os.getenv("RUEE_HOME")



if(RUEE_HOME == None):
    sys.exit("RUEE_HOME not declared...")



def run_and_print(cmd):
    print(' '.join(cmd))
    subprocess.call(cmd)



# Run passes
print("="*100)
print("Running RUEE!") 
print("RUEE_HOME:",RUEE_HOME)
print("Input File:",input_file)
print("="*100)

Cmd = ["opt-6.0","-mem2reg" , sys.argv[1], "-o",work_dir+"/"+input_file_name+"_M2R.bc"]
run_and_print(Cmd)

Cmd = ["opt-6.0","-load",RUEE_HOME+"/build/src/libLLVMLoopInvar.so" ,"-loop-invar",
    work_dir+"/"+input_file_name+"_M2R.bc", "-o",work_dir+"/"+input_file_name+"_LI.bc"]
run_and_print(Cmd)

Cmd = ["opt-6.0","-load",RUEE_HOME+"/build/src/libLLVMConstFold.so" ,"-const-fold",
    work_dir+"/"+input_file_name+"_LI.bc", "-o",work_dir+"/"+input_file_name+"_CF.bc"]
run_and_print(Cmd)


print("="*100)


# Produce human readible .ll files
print("Producing .ll files!")
subprocess.call(["cp",sys.argv[1],work_dir])

Cmd = ["llvm-dis-6.0",sys.argv[1],"-o",work_dir+"/"+input_file_name+".ll"]
run_and_print(Cmd)

Cmd = ["llvm-dis-6.0",work_dir+"/"+input_file_name+"_M2R.bc"]
run_and_print(Cmd)

Cmd = ["llvm-dis-6.0",work_dir+"/"+input_file_name+"_LI.bc"]
run_and_print(Cmd)

Cmd = ["llvm-dis-6.0",work_dir+"/"+input_file_name+"_CF.bc"]
run_and_print(Cmd)

print("="*100)


print("Producing binary!")

Cmd = ["llc-6.0","-filetype=obj", work_dir+"/"+input_file, "-o", work_dir+"/"+input_file_name+".o"]
run_and_print(Cmd)

Cmd = ["clang-6.0", work_dir+"/"+input_file_name+".o", "-o", work_dir+"/"+input_file_name+"_ORIG"]
run_and_print(Cmd)


Cmd = ["llc-6.0","-filetype=obj", work_dir+"/"+input_file_name+"_LI.bc", "-o", work_dir+"/"+input_file_name+"_LI.o"]
run_and_print(Cmd)

Cmd = ["clang-6.0", work_dir+"/"+input_file_name+"_LI.o", "-o", work_dir+"/"+input_file_name+"_LI"]
run_and_print(Cmd)

Cmd = ["llc-6.0","-filetype=obj", work_dir+"/"+input_file_name+"_CF.bc", "-o", work_dir+"/"+input_file_name+"_CF.o"]
run_and_print(Cmd)

Cmd = ["clang-6.0", work_dir+"/"+input_file_name+"_CF.o", "-o", work_dir+"/"+input_file_name+"_CF"]
run_and_print(Cmd)







