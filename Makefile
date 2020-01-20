INCLUDE = ./include
OBJS = main.o Adaboost.o Bayes.o CART.o CRF-CWS.o DNN.o DTree.o GMM.o Hash.o HMM-CWS.o HMM.o kMeans.o  KNN.o LineReg.o LogReg.o MDP.o ME.o MEMM-CWS.o PCA.o RF.o SoftMaxReg.o SVD.o SVM.o LoadData.o Matrix.o MatrixOpe.o
H = ./include/Adaboost.h ./include/Bayes.h ./include/CART.h ./include/DNN.h ./include/DTree.h ./include/GMM.h ./include/Hash.h  ./include/HMM.h ./include/kMeans.h  ./include/KNN.h ./include/LineReg.h ./include/LogReg.h ./include/MDP.h ./include/ME.h ./include/PCA.h ./include/RF.h ./include/SoftMaxReg.h ./include/SVD.h ./include/SVM.h

HH = ./include/LoadData.h ./include/Matrix.h ./include/MatrixOpe.h
CPP = LoadData.cpp Matrix.cpp Matrix.cpp

ifndef CXX
CXX = $(CXX) $(CXXFLAGS)
endif
CXXFLAGS=-pipe \
         -g \
  		 -W \
  		 -Wall \
  		 -fPIC \
  		 -std=c++11 \
  		 -fno-omit-frame-pointer \
  		 -fpermissive \
  		 -O3 \
  		 -ffast-math

INCPATH=-I./include/ \
		-I./include/familia \
  		-I./third_party/include \
		-I$(PYTHON_INCLUDE)

ML : $(OBJS)
	$(CXX) $(CXXFLAGS) -o ML $(OBJS) 
main.o : main.cpp  
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) main.cpp 
Adaboost.o : Adaboost.cpp 
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) Adaboost.cpp
Bayes.o : Bayes.cpp 
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) Bayes.cpp
CART.o : CART.cpp ./include/CART.h
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) CART.cpp
CRF-CWS.o : CRF-CWS.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) CRF-CWS.cpp
DNN.o : DNN.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) DNN.cpp
DTree.o : DTree.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) DTree.cpp 
GMM.o : GMM.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) GMM.cpp
Hash.o : Hash.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) Hash.cpp
HMM-CWS.o : HMM-CWS.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) HMM-CWS.cpp
HMM.o : HMM.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) HMM.cpp
kMeans.o : kMeans.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) kMeans.cpp
KNN.o : KNN.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) KNN.cpp
LineReg.o : LineReg.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) LineReg.cpp
LogReg.o : LogReg.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) LogReg.cpp
MDP.o : MDP.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) MDP.cpp
ME.o : ME.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) ME.cpp
MEMM-CWS.o : MEMM-CWS.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) MEMM-CWS.cpp
PCA.o : PCA.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) PCA.cpp
RF.o : RF.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) RF.cpp
SoftMaxReg.o : SoftMaxReg.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) SoftMaxReg.cpp
SVD.o : SVD.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) SVD.cpp
SVM.o : SVM.cpp
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) SVM.cpp
LoadData.o : LoadData.cpp $(HH)
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) LoadData.cpp
Matrix.o : Matrix.cpp $(HH)
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) Matrix.cpp
MatrixOpe.o : MatrixOpe.cpp $(HH)
	$(CXX) $(CXXFLAGS) -c -I $(INCLUDE) MatrixOpe.cpp
.PHONY : clean
clean :
	rm $(OBJS)

