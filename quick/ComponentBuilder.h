/// @file
/// Dynamically build QML code
///
// Copyright (c) 2017 Thomas Geymayer <tomgey@gmail.com>

#pragma once

#include <QByteArray>
#include <QStringList>
#include <QQmlComponent>

#include <memory>

class QQmlEngine;

namespace quick
{

  class ComponentBuilder
  {
    public:
      /**
       * @brief Set engine which will be used to create QQmlComponents
       * @param engine  Qml engine
       */
      static void setEngine(QQmlEngine* engine);

      /**
       * @brief Append data to the QML code
       */
      ComponentBuilder& append(const QByteArray& data);
      ComponentBuilder& append(const QString& str);
      ComponentBuilder& append(const QStringList& list);

      template<typename T>
      ComponentBuilder& append(const T& data)
      {
        mQmlCode.append(QString("%1").arg(data));
        return *this;
      }

      template<typename ... Args>
      ComponentBuilder& append(const Args& ... tokens)
      {
        ignore_return({append(tokens)...});
        return *this;
      }

      template<typename ... Args>
      ComponentBuilder& line(const Args& ... tokens)
      {
        return append(tokens..., '\n');
      }

      /**
       * @brief Add an import statement
       */
      ComponentBuilder& import(const QString& stmt);

      /**
       * @brief Begin an object definition
       * @see endObject()
       */
      ComponentBuilder& beginObject(const QString& name);

      /**
       * @brief End an object definition
       * @see beginObject()
       */
      ComponentBuilder& endObject();

      ComponentBuilder& dump();

      std::unique_ptr<QQmlComponent> create();

    protected:
      static QQmlEngine *mEngine; ///!< Engine to instantiate
                                  ///   components within

      QByteArray mQmlCode;  ///<! Code of the component being built

      template<class T>
      void ignore_return(std::initializer_list<T>){}
  };

}
