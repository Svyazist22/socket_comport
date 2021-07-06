/**
  @brief Method: calling specific method of the specified object
  @details Простая альтернатива std::function<ret(args)> или ret(*function)(args)
  Используется для передачи вызова/события/сигнала из вложенного объекта в родительский объект.

  === Использование ===
  В родительском объекте необходимо привязать свой метод:
      childObject.onSetString.set(this, &ParentClass::myMethod);

  Сам метод должен иметь тип
      bool ParentClass::myMethod(const char* a, const char* b);

  Во вложенном объекте необходимо описать переменную:
      Method<bool, const char*, const char*> onSetStrings;

  Во вложенном объекте необходимо вызывать:
      if (onSetStrings) { bool result = onSetStrings("abc", "xyz"); ... }

  @author Nikita Shishkov
  */

#pragma once

template<typename Ret, typename... Args>
class Method
{
protected:
    class AnyClass;
    typedef Ret(AnyClass::*AnyMethodPointer)(Args...);

    /// Указатель на объект
    AnyClass* object = nullptr;

    /// Указатель на метод
    AnyMethodPointer methodPointer = nullptr;

public:
    /// Конструктор
    inline Method ()
    {
        methodPointer = nullptr;
    }

    /// Setter
    template<class T>
    inline Method(T* _object, Ret (T::*_methodPointer)(Args...))
    {
        object = (AnyClass*)_object;
        methodPointer = (AnyMethodPointer)_methodPointer;
    }

    /// ReSetter
    inline void reset()
    {
        methodPointer = nullptr;
    }

    /// Проверка, используется ли данный Method
    inline operator bool () const
    {
        return methodPointer != nullptr;
    }

    /// Вызвать Method
    inline Ret operator () (Args... args) const
    {
        if (methodPointer)
            return (object->*methodPointer)(args...);
        return Ret();
    }
};

// Синтаксис С++ не позволяет автоматически определять типы шаблонного класса по типам аргументов констурктора.
//   server.onMessage = Method(this, &MyObject::messageRecieved);
// Поэтому введена эта функция, которая позволяет так писать
//   server.onMessage = method(this, &MyObject::messageRecieved);

template<class T, typename Ret, typename... Args>
inline Method<Ret, Args...> method (T* _object, Ret (T::*_methodPointer)(Args...))
{
    return Method<Ret, Args...>(_object, _methodPointer);
}
