#pragma once

class IRenderer
{
public:
	IRenderer() { }
	~IRenderer() { }

	virtual void Prepare() = 0;
	virtual void Render() = 0;

protected:

};