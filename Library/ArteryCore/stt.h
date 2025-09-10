// #ifndef _STT_H_
// #define _STT_H_

// /*!
//   @brief  Стандартные типы для нашего ПО
// */


// /// @brief Битовое поле с методами.
// /// @tparam T :: принимает типы от i/u_8 до i/u_64
// template<typename T>
// class BitField {
//   // static_assert( 
//   //   // typeid(T) == typeid(uint8_t) 
//   //   // || 
//   //   // std::is_same<T, uint8_t   >::value   || 
//   //   // std::is_same<T, int16_t   >::value   || 
//   //   // std::is_same<T, uint16_t  >::value   || 
//   //   // std::is_same<T, int32_t   >::value   || 
//   //   // std::is_same<T, uint32_t  >::value   || 
//   //   // std::is_same<T, int64_t   >::value   ||
//   //   // std::is_same<T, uint64_t  >::value  
//   //   , "template instantiation of Class are not integer"
//   // );
//   protected:
//     T field {};
//   public:
//     BitField();
//     BitField(T value);
//     T GetValue();
//     void SetValue(T value);
//     bool GetState(uint8_t N);
//     void SetState(uint8_t N, bool state);
// };

// template<typename T, typename TLen, size_t Size>
// class Array {
//   private:
//     T arr[Size];
//     TLen index {};          // Текущий индекс / кол-во элементов
//   public:
//     bool SetValue(T value);
//     bool SetValue(T value, TLen _index);
//     T GetValue(TLen _index);
//     TLen GetIndex();
//     void Clear();
// };

// // #include "stt.impl"



// #endif // _STT_H_