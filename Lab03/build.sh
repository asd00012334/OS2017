g++ main.cpp -pthread -o main
g++ t_recv.cpp -o recv
g++ test.cpp -o test
./test > input
echo input | ./main


echo out1 checking
./recv < 'output1.txt'
echo out2 checking
./recv < 'output2.txt'

