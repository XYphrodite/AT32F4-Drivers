// #include <cstdint>
// #include <cassert>
// #include <cstring>
// #include <stt.h>


// template<typename T>
// BitField<T>::BitField() {}

// template<typename T>
// BitField<T>::BitField(T value) : field(value) {}

// template<typename T>
// T BitField<T>::GetValue() {
//   return field;
// }

// template<typename T>
// void BitField<T>::SetValue(T value) {
//   field = value;
// }

// template<typename T>
// bool BitField<T>::GetState(uint8_t N) {
//   if(N > (sizeof(T) * 8) )
//     return false;
//   return static_cast<bool>( (field >> N) & 0x01 );
// }

// template<typename T>
// void BitField<T>::SetState(uint8_t N, bool state) {
//   if( N > (sizeof(T)*8) )
//     return;
//   field &= ~(static_cast<T>(0x1U) << N);
//   field |= (static_cast<T>(state) << N) & 0x1U;
// }



// template<typename T, typename TLen, size_t Size>
// bool Array<T, TLen, Size>::SetValue(T value) {
//   if(index > (sizeof(arr) - 1))
//     return false;
//   arr[index++] = value;
//   return true;
// }

// template<typename T, typename TLen, size_t Size>
// bool Array<T, TLen, Size>::SetValue(T value, TLen _index) {
//   if(_index > (sizeof(arr) - 1))
//     return false;
//   arr[_index] = value;
//   return true;
// }

// template<typename T, typename TLen, size_t Size>
// T Array<T, TLen, Size>::GetValue(TLen _index) {
//   if(_index > (sizeof(arr) - 1))
//     return 0;
//   return arr[_index];
// }
// template<typename T, typename TLen, size_t Size>
// TLen Array<T, TLen, Size>::GetIndex() {
//   return index;
// }

// template<typename T, typename TLen, size_t Size>
// void Array<T, TLen, Size>::Clear() {
//   memset((void*)arr, 0, sizeof(arr));
// }