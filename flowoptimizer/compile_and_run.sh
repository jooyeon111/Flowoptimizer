g++ -std=c++17 main.cpp Layer_analysis.cpp Dataflow.cpp Directive.cpp util.cpp table.cpp -o flow_optimizer
mv flow_optimizer ../maestro

cd ..
cd maestro
./flow_optimizer 