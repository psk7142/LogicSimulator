#include "LibraryBoxComponent.h"

CLibraryBox::CLibraryBox()
{
	componentList.resize(10);
	inputPinIDList.resize(10);
	outputPinIDList.resize(10);
	componentTypeList.resize(10, COMPONENT_TYPE_NONE);
	connnectionGraph.resize(10);
	connectedTerminalInfo.resize(10);
}

CLibraryBox::CLibraryBox(CLibraryBox & object)
	:CComponentObject(object)
{

}

CLibraryBox::~CLibraryBox()
{
	//동적 할당되는 부분을 모두 해제함
	for (int i = 0; i < componentList.size(); i++) {
		if (componentTypeList[i] == COMPONENT_TYPE_NONE) {
			delete componentList[i];
		}
	}
}

COMPONENT_ID CLibraryBox::getNewComponetID(COMPONENT_TYPE componentType)
{
	for (int i = 1; i < componentTypeList.size(); i++) {
		if (componentTypeList[i] == COMPONENT_TYPE_NONE) {
			componentTypeList[i] = componentType;
			return i;
		}
	}
	componentTypeList.push_back(componentType);
	return componentTypeList.size()-1;
}

void CLibraryBox::deleteComponentID(COMPONENT_ID deleteId)
{
	componentTypeList[deleteId] = COMPONENT_TYPE_NONE;
}

void CLibraryBox::loadLibrarybox(std::vector<LIBRARY_BOX_DATA>& LibraryBoxData)
{
	for (int i = 0; i < LibraryBoxData.size(); i++) {
		LibraryBoxData[i];
	}
}

void CLibraryBox::setSingleInputPinValue(bool _inputValue, int _inputPinNumber)
{
	COMPONENT_ID inputPinID;
	inputPinID = inputPinIDList[_inputPinNumber];
	CComponentObject* componentObject = ((CComponentObject*)componentList[inputPinID]);
	CInputPinComponent* inputPinObject = ((CInputPinComponent*)componentObject);
	inputPinObject->setValue(_inputValue);
}

bool CLibraryBox::getSingleInputPinValue(int _inputPinNumber)
{
	COMPONENT_ID inputPinID;
	inputPinID = inputPinIDList[_inputPinNumber];
	CComponentObject* componentObject = ((CComponentObject*)componentList[inputPinID]);
	CInputPinComponent* inputPinObject = ((CInputPinComponent*)componentObject);
	return inputPinObject->getValue();

}

bool CLibraryBox::getSingleOutputPinValue(int _outPutPinNumber)
{
	COMPONENT_ID outputPinID;
	outputPinID = inputPinIDList[_outPutPinNumber];
	CComponentObject* componentObject = ((CComponentObject*)componentList[outputPinID]);
	CInputPinComponent* outputPinObject = ((CInputPinComponent*)componentObject);
	return outputPinObject->getValue();
}

bool CLibraryBox::addComponent(COMPONENT_INFO & componentInfo)
{
	COMPONENT_TYPE newComponentType;
	COMPONENT_ID newComponentID;

	newComponentType = componentInfo.componentType;

	newComponentID = getNewComponetID(newComponentType);
	componentInfo.componentID = newComponentID;
	// 더 추가 해야 할경우 확장함
	if (componentList.size() <= newComponentID) {
		componentList.resize(componentList.size() + 10);
		connnectionGraph.resize(connnectionGraph.size() + 10);
	}

	switch (newComponentType)
	{
		// input component
	case COMPONENT_TYPE_INPUT_PIN:
		inputPinIDList.push_back(newComponentID);
		componentList[newComponentID] = new CInputPinComponent();


		break;
	case COMPONENT_TYPE_CLOCK:
		componentList[newComponentID] = new CClockComponent();
		break;
	case COMPONENT_TYPE_ONE_BIT_SWITCH:
		componentList[newComponentID] = new COneBitSwitchComponent();

		break;

		//logic gate component
	case COMPONENT_TYPE_AND_GATE:
		componentList[newComponentID] = new CANDGateComponent();

		break;
	case COMPONENT_TYPE_OR_GATE:
		componentList[newComponentID] = new CORGateComponent();

		break;
	case COMPONENT_TYPE_NOT_GATE:
		componentList[newComponentID] = new CNOTGateComponent();

		break;
	case COMPONENT_TYPE_NAND_GATE:
		componentList[newComponentID] = new CNANDGateComponent();

		break;
	case COMPONENT_TYPE_NOR_GATE:
		componentList[newComponentID] = new CNORGateComponent();

		break;
	case COMPONENT_TYPE_XOR_GATE:
		componentList[newComponentID] = new CXORGateComponent();

		break;

		//wire component
	case COMPONENT_TYPE_WIRE:
		componentList[newComponentID] = new CWireComponent();
		break;

		//output component
	case COMPONENT_TYPE_7SEGMENT:
		componentList[newComponentID] = new C7SegmentComponent();

		break;
	case COMPONENT_TYPE_OUTPUT_PIN:
		componentList[newComponentID] = new COutputPinComponent();

		break;
	case COMPONENT_TYPE_ONE_BIT_LAMP:
		componentList[newComponentID] = new COneBitLampComponent();

		break;

		//생성할수없음
	default:
		deleteComponentID(newComponentID);
		return false;
		break;
	}

	//생성 성공
	return true;
}

bool CLibraryBox::deleteComponent(COMPONENT_ID _componentID)
{
	//존재하지 않은 부품을 삭제하려고 하면 false 반환
	if (componentTypeList[_componentID] == COMPONENT_TYPE_NONE) {
		return false;
	}

	//연결된 부품을 찾아서 연결된부품에서 지우려는 부품의 연결을 끊고 사용하는 단자를 갱신함
	COMPONENT_ID nextID;
	COMPONENT_CONENTION_INFO* nextConnectionInfo;
	COMPONENT_CONENTION_INFO* deleteConnetionInfo;

	//연결된 반편 부품의 그래프와 터미널 정보를 갱신함
	for (int i = 0; i < connnectionGraph[_componentID].size(); i++) {
		nextConnectionInfo = &connnectionGraph[_componentID][i];
		//if (componentID[nextConnectionInfo->componentID] == COMPONENT_TYPE_WIRE &&);
		nextID = nextConnectionInfo->componentID;

		//연결된 반대편의 부품에서 터미널정보를 갱신함
		for (int j = 0; j < connectedTerminalInfo[nextID].size(); j++) {
			deleteConnetionInfo = &connectedTerminalInfo[nextID][j];
			if (deleteConnetionInfo->componentID == nextConnectionInfo->componentID) {
				connectedTerminalInfo[nextID].erase(connectedTerminalInfo[nextID].begin() + j);
				break;
			}
		}
		//연결된 반대편의 부품에서 그래프연결을 끊음
		for (int j = 0; j < connnectionGraph[nextID].size(); j++) {
			deleteConnetionInfo = &connnectionGraph[nextID][j];
			if (deleteConnetionInfo->componentID == _componentID) {
				connnectionGraph[nextID].erase(connnectionGraph[nextID].begin() + j);
				break;
			}
		}
	}

	//지워지는 부품의 그래프와 터미널 정보를 갱신함
	connnectionGraph[_componentID].clear();
	connectedTerminalInfo[_componentID].clear();

	//컴포넌트 아이디 삭제
	deleteComponentID(_componentID);
	delete componentList[_componentID];

	return true;
}

bool CLibraryBox::connectComponentAndWire(COMPONENT_CONENTION_INFO & ComponentInfo, COMPONENT_CONENTION_INFO & wireInfo)
{
	// 존재하지 않는 부품이랑 연결하려할떄 연결못하게함
	if (componentTypeList[ComponentInfo.componentID] == COMPONENT_TYPE_NONE ||
		componentTypeList[wireInfo.componentID] == COMPONENT_TYPE_NONE) {
		return false;
	}

	COMPONENT_CONENTION_INFO* connectionInfo;
	// 연결하려는 부품의 단자가 이미사용중인지 검사
	for (int i = 0; i < connectedTerminalInfo[ComponentInfo.componentID].size(); i++) {
		connectionInfo = &connectedTerminalInfo[ComponentInfo.componentID][i];
		if (connectionInfo->componentID == ComponentInfo.componentID &&
			connectionInfo->terminalNumber == ComponentInfo.terminalNumber&&
			connectionInfo->terminalType == ComponentInfo.terminalType) {
			return false;
		}
	}
	// 연결하려는 줄의 단자가 이미 사용중인자 검사
	for (int i = 0; i < connectedTerminalInfo[wireInfo.componentID].size(); i++) {
		connectionInfo = &connectedTerminalInfo[wireInfo.componentID][i];
		if (connectionInfo->componentID == wireInfo.componentID &&
			connectionInfo->terminalNumber == wireInfo.terminalNumber&&
			connectionInfo->terminalType == wireInfo.terminalType) {
			return false;
		}
	}

	//양뱡향으로 연결함
	connnectionGraph[ComponentInfo.componentID].push_back(wireInfo);
	connectedTerminalInfo[ComponentInfo.componentID].push_back(ComponentInfo);
	connnectionGraph[wireInfo.componentID].push_back(ComponentInfo);
	connectedTerminalInfo[wireInfo.componentID].push_back(wireInfo);
	return true;
}

bool CLibraryBox::disconnectComponentAndWire(COMPONENT_CONENTION_INFO & ComponentInfo, COMPONENT_CONENTION_INFO & wireInfo)
{
	// 존재하지 않는 부품이랑 분리하려할떄 연결못하게함
	if (componentTypeList[ComponentInfo.componentID] == COMPONENT_TYPE_NONE ||
		componentTypeList[wireInfo.componentID] == COMPONENT_TYPE_NONE) {
		return false;
	}

	COMPONENT_CONENTION_INFO* currrentComponent;
	//부품에서 와이어방향으로 연결된것을 끊음
	for (int i = 0; i < connnectionGraph[ComponentInfo.componentID].size(); i++) {
		currrentComponent = &connnectionGraph[ComponentInfo.componentID][i];
		if (currrentComponent->componentID == wireInfo.componentID &&
			currrentComponent->terminalNumber == wireInfo.terminalNumber &&
			currrentComponent->terminalType == wireInfo.terminalType) {
			connnectionGraph[ComponentInfo.componentID].erase(connnectionGraph[ComponentInfo.componentID].begin() + i);
			break;
		}
	}

	COMPONENT_CONENTION_INFO* currrentWire;
	//와이어에서 부품 방향으로 연결된것을 끊음
	for (int i = 0; i < connnectionGraph[wireInfo.componentID].size(); i++) {
		currrentWire = &connnectionGraph[wireInfo.componentID][i];
		if (currrentWire->componentID == ComponentInfo.componentID &&
			currrentWire->terminalNumber == ComponentInfo.terminalNumber &&
			currrentWire->terminalType == ComponentInfo.terminalType) {
			connnectionGraph[wireInfo.componentID].erase(connnectionGraph[wireInfo.componentID].begin() + i);
			break;
		}
	}

	COMPONENT_CONENTION_INFO* connectionInfo;
	//사용했던 부품의 단자의 목록을 갱신함
	for (int i = 0; i < connectedTerminalInfo[ComponentInfo.componentID].size(); i++) {
		connectionInfo = &connectedTerminalInfo[ComponentInfo.componentID][i];
		if (connectionInfo->componentID == ComponentInfo.componentID &&
			connectionInfo->terminalNumber == ComponentInfo.terminalNumber &&
			connectionInfo->terminalType == ComponentInfo.terminalType) {
			connectedTerminalInfo[ComponentInfo.componentID].erase(connectedTerminalInfo[ComponentInfo.componentID].begin() + i);
			break;
		}
	}

	//사용했던 와이어의 단자를 갱신함
	for (int i = 0; i < connectedTerminalInfo[wireInfo.componentID].size(); i++) {
		connectionInfo = &connectedTerminalInfo[wireInfo.componentID][i];
		if (connectionInfo->componentID == wireInfo.componentID &&
			connectionInfo->terminalNumber == wireInfo.terminalNumber &&
			connectionInfo->terminalType == wireInfo.terminalType) {
			connectedTerminalInfo[wireInfo.componentID].erase(connectedTerminalInfo[wireInfo.componentID].begin() + i);
			break;
		}
	}

	return true;
}

bool CLibraryBox::connectWireAndWire(COMPONENT_CONENTION_INFO & wireA, COMPONENT_CONENTION_INFO & wireB)
{
	//둘중 하나라도 존재하지않는 부품일떄
	if (componentTypeList[wireA.componentID] == COMPONENT_TYPE_NONE ||
		componentTypeList[wireB.componentID] == COMPONENT_TYPE_NONE) {
		return false;
	}
	COMPONENT_CONENTION_INFO* wire;
	//이미 연결했었는지 확인함
	for (int i = 0; i < connnectionGraph[wireA.componentID].size(); i++) {
		wire = &connnectionGraph[wireA.componentID][i];
		if (wire->componentID == wireB.componentID &&
			wire->terminalNumber == wireB.terminalNumber &&
			wire->terminalType == wireB.terminalType) {
			return false;
		}
	}
	for (int i = 0; i < connnectionGraph[wireB.componentID].size(); i++) {
		wire = &connnectionGraph[wireB.componentID][i];
		if (wire->componentID == wireA.componentID &&
			wire->terminalNumber == wireA.terminalNumber &&
			wire->terminalType == wireA.terminalType) {
			return false;
		}
	}

	//연결함
	connnectionGraph[wireA.componentID].push_back(wireB);
	connnectionGraph[wireB.componentID].push_back(wireA);
	connectedTerminalInfo[wireA.componentID].push_back(wireA);
	connectedTerminalInfo[wireB.componentID].push_back(wireB);

	return true;
}

bool CLibraryBox::disconnectWireAndWire(COMPONENT_CONENTION_INFO & wireA, COMPONENT_CONENTION_INFO & wireB)
{
	//존재하지 와이어를 분리하려는지 검사함
	if (componentTypeList[wireA.componentID] == COMPONENT_TYPE_NONE ||
		componentTypeList[wireB.componentID] == COMPONENT_TYPE_NONE) {
		return false;
	}

	//연결 그래프에서 서로 와이어를 분리함
	COMPONENT_CONENTION_INFO* currentWire;
	for (int i = 0; i < connnectionGraph[wireA.componentID].size(); i++) {
		currentWire = &connnectionGraph[wireA.componentID][i];
		if (currentWire->componentID == wireB.componentID &&
			currentWire->terminalNumber == wireB.terminalNumber &&
			currentWire->terminalType == wireB.terminalType) {
			connnectionGraph[wireA.componentID].erase(connnectionGraph[wireA.componentID].begin() + i);
break;
		}
	}
	for (int i = 0; i < connnectionGraph[wireB.componentID].size(); i++) {
		currentWire = &connnectionGraph[wireB.componentID][i];
		if (currentWire->componentID == wireA.componentID &&
			currentWire->terminalNumber == wireA.terminalNumber &&
			currentWire->terminalType == wireA.terminalType) {
			connnectionGraph[wireB.componentID].erase(connnectionGraph[wireB.componentID].begin() + i);
			break;
		}
	}

	COMPONENT_CONENTION_INFO* terminalInfo;
	//서로 사용하는 단자를 갱신함
	for (int i = 0; i < connnectionGraph[wireA.componentID].size(); i++) {
		terminalInfo = &connnectionGraph[wireA.componentID][i];
		if (terminalInfo->componentID == wireA.componentID &&
			terminalInfo->terminalNumber == wireA.terminalNumber &&
			terminalInfo->terminalType == wireA.terminalType) {
			connnectionGraph[wireA.componentID].erase(connnectionGraph[wireA.componentID].begin() + i);
			break;
		}
	}
	for (int i = 0; i < connnectionGraph[wireB.componentID].size(); i++) {
		terminalInfo = &connnectionGraph[wireB.componentID][i];
		if (terminalInfo->componentID == wireB.componentID &&
			terminalInfo->terminalNumber == wireB.terminalNumber &&
			terminalInfo->terminalType == wireB.terminalType) {
			connnectionGraph[wireB.componentID].erase(connnectionGraph[wireB.componentID].begin() + i);
			break;
		}
	}

	return true;
}



/*
메모
그래프 객체를 필요한 받아와서 정해주기함
vector 로 하지만 일일이 찾아서 삭제 하지말고 그냥 다만들어놓고 하기


*/
bool CLibraryBox::updateCircuit()
{
	//만약 진동 회로 이면 멈추그만한다...

	//bfs 방식으로 한다
	std::queue<COMPONENT_ID> queue;
	std::vector< std::vector<bool> > componentValueVector;

	//값을 저장해놓음
	//필요한 사이즈 많큼 할당해놓음
	componentValueVector.resize(componentTypeList.size());


	for (int i = 0; i < componentTypeList.size(); i++) {




	}

	//시작점인 인풋핀의 component id 를 큐에다가 넣는다
	for (int i = 0; i < inputPinIDList.size(); i++) {
		queue.push(inputPinIDList[i]);
	}


	//큐를 돌면서 부품들의 내부정보를 갱신한다
	while (queue.empty()) {




	}
	//탐색을 할떄 아웃풋 방향으로 들어가면 더 멈춤
	//또는 인풋으로 들어갔는데 나오는 아웃풋이 달라지면  달라지는 아웃풋에대해
	//큐에 추가한다
	//와이어는 인풋 아웃풋이 상관이없음

	//오큘레이션 디텍션 어떻게 해야함?


	return false;
}

void CLibraryBox::printstatus()
{
	printf("\n");
	printf("################################\n");
	printf("simultor status \n");
	printf("################################\n");

	//현재 들어있는 컴포넌트들을 과 상태값을 출력한다;

	for (int i = 0; i < componentTypeList.size(); i++) {
		//없는거는 생략함
		if (componentTypeList[i] == COMPONENT_TYPE_NONE){
			continue;
		}
		printf("componentID : %d component type: %d\n", i, componentTypeList[i]);
		printf("component information:\n");
		//줄일때
		COMPONENT_CONENTION_INFO* info;
		printf("using terminal infoation:\n");
		for (int j = 0; j<connectedTerminalInfo[i].size(); j++) {
			info = &connectedTerminalInfo[i][j];
			printf("terminal type : %d  terminal number : %d\n", 
				info->terminalType, info->terminalNumber);
		}
		printf("connnected info\n");
		for (int j = 0; j<connnectionGraph[i].size(); j++) {
			info = &connnectionGraph[i][j];
			printf("connnected to component ID : %d, terminal type : %d	terminal number : %d\n",
				info->componentID, info->terminalType, info->terminalNumber);
		}
		printf("\n");
	}




	printf("################################\n");
	printf("end\n");
	printf("################################\n");
	printf("\n\n");
}


