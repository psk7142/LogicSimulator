#pragma once
#include "ComponentObject.h"

class CNANDGateComponent :public CComponentObject {
private:
	bool inputValue1;
	bool inputValue2;
	bool outputValue;

public:
	CNANDGateComponent();
	CNANDGateComponent(CNANDGateComponent& object);
	virtual ~CNANDGateComponent();

	void setInputValue1(bool _inputValue1);
	bool getInputValue1();
	void setInputValue2(bool _inputValue2);
	bool getInputValue2();
	bool getOutputValue();
	void updateOutputValue();


};