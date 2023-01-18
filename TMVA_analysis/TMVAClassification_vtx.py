from ROOT import TMVA, TFile, TTree, TCut
from subprocess import call
from os.path import isfile
 
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Activation, Conv1DTranspose, Flatten, Input
from tensorflow.keras.optimizers import SGD
 
# Setup TMVA
TMVA.Tools.Instance()
TMVA.PyMethodBase.PyInitialize()
 
output = TFile.Open('TMVA_vtx.root', 'UPDATE')
factory = TMVA.Factory('TMVAClassification', output,
                       '!V:!Silent:Color:DrawProgressBar:Transformations=D,G:AnalysisType=Classification')
 
#Load Data 
#data = TFile.Open('scout_probe_small.root')
#signal = data.Get('upsilon_tree')
#background = data.Get('bckg_tree')
data = TFile.Open("/work/submit/juliush/UROP_darkphoton/test/CMSSW_12_4_3/src/UROP_darkphoton/systematics/tagnprobe_input_era5.root")
signal = data.Get('tpTree/upsilon_tree')
background = data.Get('tpTree/bckg_tree')


dataloaders = []
#variables = ["vtxchi2", "PVd", "vtxErr"]
variables = ["vtxchi2", "IP"]
#variables = ["IP", "PVd"]
#variables = ["vtxchi2", "IP", "PVd", "vtxErr"]

#Generate Models
for i in range( len(variables)):
    #if i<2:
    #   continue
    name = "variables"
    for j in range(i+1):
       name += "_" + variables[j]
    dataloader = TMVA.DataLoader(name)
    for j in range(i+1):
        dataloader.AddVariable(variables[j])
    dataloader.AddSignalTree(signal, 1.0)
    dataloader.AddBackgroundTree(background, 1.0)
    dataloader.PrepareTrainingAndTestTree(TCut(''),
                                          'nTrain_Signal=50000:nTrain_Background=50000:SplitMode=Random:NormMode=NumEvents:!V')
    dataloaders.append(dataloader)

# Define model
model = Sequential()
model.add(Dense(64, activation='relu', input_dim=5))
model.add(Flatten())
model.add(Dense(128, activation='relu'))
model.add(Dense(32, activation='relu'))
model.add(Dense(2, activation='softmax'))
 
# Set loss and optimizer
model.compile(loss='categorical_crossentropy',
              optimizer=SGD(learning_rate=0.01), metrics=['accuracy', ])
 
# Store model to file
model.save('modelClassification.h5')
model.summary()
 
# Book methodsi
for dataloader in dataloaders:
    factory.BookMethod(dataloader, TMVA.Types.kBDT, "BDT",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
    #factory.BookMethod(dataloader, TMVA.Types.kFisher, 'Fisher',
    #               '!H:!V:Fisher:VarTransform=D,G')
    #factory.BookMethod(dataloader, TMVA.Types.kPyKeras, 'PyKeras',
    #               'H:!V:VarTransform=D,G:FilenameModel=modelClassification.h5:FilenameTrainedModel=trainedModelClassification.h5:NumEpochs=50:BatchSize=32')
 
# Run training, test and evaluation
factory.TrainAllMethods()
factory.TestAllMethods()
factory.EvaluateAllMethods()
