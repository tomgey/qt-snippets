// Dynamically build QML code
//
// Copyright (C) 2017 Thomas Geymayer <tomgey@gmail.com>

#include "ComponentBuilder.h"

#include <QQmlEngine>

using namespace quick;

QQmlEngine* ComponentBuilder::mEngine {nullptr};

void ComponentBuilder::setEngine(QQmlEngine* engine)
{
	mEngine = engine;
}

ComponentBuilder& ComponentBuilder::append(const QByteArray& data)
{
	mQmlCode.append(data);
	return *this;
}

ComponentBuilder& ComponentBuilder::append(const QString& str)
{
	mQmlCode.append(str);
	return *this;
}

ComponentBuilder& ComponentBuilder::append(const QStringList& list)
{
	return append('[', list.join(", "), ']');
}

ComponentBuilder& ComponentBuilder::import(const QString& stmt)
{
	return line("import ", stmt);
}

ComponentBuilder& ComponentBuilder::beginObject(const QString& name)
{
	return line(name, " {");
}

ComponentBuilder& ComponentBuilder::endObject()
{
	return line('}');
}

ComponentBuilder& ComponentBuilder::dump()
{
	qDebug(mQmlCode.data());
	return *this;
}

std::unique_ptr<QQmlComponent> ComponentBuilder::create()
{
	auto component = std::make_unique<QQmlComponent>(mEngine);
	component->setData(std::move(mQmlCode), QUrl("qrc:/"));

	QQmlEngine::setObjectOwnership(component.get(), QQmlEngine::CppOwnership);
	return component;
}
