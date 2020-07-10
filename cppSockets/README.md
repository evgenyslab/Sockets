# install gtest 

```bash
git clone https://github.com/google/googletest.git
cd googletest 
git checkout release-1.10.0
mkdir build 
cd build 
cmake ..
make -j 8
sudo make install 

echo "export CPLUS_INCLUDE_PATH=/usr/local/include" >> ~/.bash_profile
echo "export LIBRARY_PATH=/usr/local/lib" >> ~/.bash_profile
 
source ~/.bash_profile 

```

[ref](https://medium.com/@alexanderbussan/getting-started-with-google-test-on-os-x-a07eee7ae6dc)


To run the test suite...

