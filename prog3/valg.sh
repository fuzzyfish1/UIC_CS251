cp -R /mnt/c/Users/Fuzzyfish/CLionProjects/CS251/251progy3/* ./
cmake CMakeLists.txt

make build >> DEBUG 2>&1

valgrind --leak-check=full --error-exitcode=1 ./runTests
EXIT_CODE=$? 

if [ $EXIT_CODE -eq 0 ]; then 
	echo true > OUTPUT
	echo 'Test passed!'
else 
	echo false > OUTPUT
	echo 'Memory leak detected!' >> DEBUG
fi
