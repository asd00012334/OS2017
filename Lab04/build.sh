g++ main.cpp -pthread -o main -g
g++ t_recv.cpp -o recv
g++ test.cpp -o test
echo generate input
./test > input.txt
echo start exececut
./main


echo out1 checking
./recv < 'output_1.txt'
echo out2 checking
./recv < 'output_2.txt'
echo out3 checking
./recv < 'output_3.txt'
echo out4 checking
./recv < 'output_4.txt'
echo out5 checking
./recv < 'output_5.txt'
echo out6 checking
./recv < 'output_6.txt'
echo out7 checking
./recv < 'output_7.txt'
echo out8 checking
./recv < 'output_8.txt'

