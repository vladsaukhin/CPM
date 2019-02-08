#pragma once

class IAnalyzer
{
public:
	virtual ~IAnalyzer(){}
	virtual void StartProcessing() = 0;
	virtual void ViewLogs() = 0;
};

