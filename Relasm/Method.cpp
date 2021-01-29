#include "Method.h"

Method::Method(const QString& accessModifier, const QString& isStatic, const QString& dataType, const QString& declClass, const QString& name, const QList<Parameter>& parameters)
	: accessModifier(flagAccessModifier[accessModifier]), isStatic(flagStatic[isStatic]), dataType(dataType), declClass(declClass), name(name), parameters(parameters)
{
}

QByteArray Method::Compile() const
{
	QByteArray code;
	QDataStream ds(&code, QIODevice::WriteOnly);

	ds << byteOpCode;
	ds << quint8(accessModifier) << quint8(isStatic);
	ds << dataType.toUtf8();
	ds << declClass.toUtf8();
	ds << name.toUtf8();
	ds << int(parameters.size());
	for (auto& item : parameters)
	{
		QByteArray compiledParameter = item.Compile();
		for (int i = 0; i < compiledParameter.size(); ++i)
		{
			ds << (quint8)compiledParameter.at(i);
		}
	}

	//body
	QByteArray byteArrayBody;
	QDataStream dsBody(&byteArrayBody, QIODevice::WriteOnly);
	for (auto& item : body)
	{
		QByteArray compiledParameter = item->Compile();
		for (int i = 0; i < compiledParameter.size(); ++i)
		{
			dsBody << (quint8)compiledParameter.at(i);
		}
	}
	ds << int(byteArrayBody.size());
	for (int i = 0; i < byteArrayBody.size(); ++i)
	{
		ds << (quint8)byteArrayBody.at(i);
	}

	return code;
}

void Method::Add(OpBase* opCode)
{
	body.push_back(opCode);
}
