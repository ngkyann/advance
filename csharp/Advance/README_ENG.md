# C++ Store Class to C# Conversion

## Project Summary

Successfully converted a comprehensive C++ template-based generic container class (`store<T, Container>`) to an idiomatic C# generic class (`Store<T>`). The conversion maintains all core functionality while adapting to C# conventions and leveraging the .NET Framework.

## Files

- **Store.cs** - Main implementation containing:
  - `ErrorManager` static class for error handling
  - `Store<T>` generic collection class
- **USAGE_EXAMPLES.cs** - Comprehensive examples demonstrating all major features

## Core Features Converted

### 1. **Error Management System**
- Thread-local error tracking using `ThreadLocal<T>`
- Dual-mode operation: Exception throwing vs. error code returning
- Error codes: -1 (OutOfRange), -2 (InvalidArgument), -3 (RuntimeError)

### 2. **Generic Container Class (Store<T>)**
- Wrapper around `List<T>` providing enhanced functionality
- Implements `IEnumerable<T>`, `IEquatable<Store<T>>`, `ICollection<T>`
- Supports collection initializer syntax: `new Store<int> { 1, 2, 3 }`

### 3. **Element Access**
- Index access: `store[0]`, `store[-1]` (Python-style negative indexing)
- Bounds-checked access: `store.At(index)`
- Properties: `First`, `Last`, `Max`, `Min`
- Tuple return: `MaxMin()` returns `(max, min)`

### 4. **Collection Operations**
- **Adding**: `PushFront()`, `PushBack()`, `Insert()`
- **Removing**: `PopFront()`, `PopBack()`, `RemoveAt()`
- **Modification**: `ReplaceAt()`, `ReplaceAll()`, `Fill()`, `Reverse()`
- **Management**: `Clear()`, `Resize()`, `Reserve()`

### 5. **Searching & Filtering**
- Existence checks: `Contains()`, `AnyOf()`, `AllOf()`, `NoneOf()`
- Count operations: `CountOf()`, `CountIf()`
- Find operations: `FindAllOf()`, `FindAllIf()` - returns positions
- Result filtering: `Filter<T>(predicate)` - returns new `Store<T>`

### 6. **Transformation**
- In-place: `Transform(Func<T,T>)` - modifies elements
- Projection: `Map<TResult>(Func<T,TResult>)` - creates new typed store
- Conversion: `ToStr()`, `ToLongLong()`, `ToDouble()`, `ToList()`, `ToArray()`

### 7. **Sorting & Deduplication**
- Basic sort: `Sort(ascending = true)`
- Custom comparers: `Sort(IComparer<T>)` or `Sort(Func<T,T,int>)`
- Deduplication: `Unique(autoSort = true)`

### 8. **String Operations**
- Join with separator: `Join(separator)` 
- Custom formatting: `Join(separator, Func<T,string> converter)`

### 9. **Slicing (Python-style)**
- 3-parameter: `Slice(start, end, step)`
- 2-parameter: `Slice(start, end)`
- Supports negative indices: `Slice(-3, -1)`

### 10. **Clone Management**
- Save state: `SetClone()`
- Restore: `RestoreClone()`
- Swap: `SwapClone()`
- Delete: `DeleteClone()`
- Retrieve: `GetClone()` - returns new `Store<T>`

### 11. **Comparison Operators**
- Full support: `<`, `>`, `<=`, `>=`, `==`, `!=`
- Lexicographical comparison
- Equality checking via `Equals()` and `IEquatable<T>`

### 12. **Range Generation**
- Static method: `Store<T>.Range(start, stop, step)`
- Simplified: `Store<T>.Range(stop)` - generates 0 to stop

## Key Differences from C++ Implementation

| Aspect | C++ | C# |
|--------|-----|-----|
| **Template Parameters** | `template<typename T, typename Container>` | Single generic `Store<T>` with internal `List<T>` |
| **Storage** | Configurable (vector, deque, list) | Always `List<T>` |
| **Memory** | Manual with `unique_ptr` | Garbage collected |
| **Error State** | Instance-based | Static `ThreadLocal<T>` |
| **Negative Index** | Not supported | Full support |
| **Slicing** | Via `operator()` | Via `Slice()` method |
| **LINQ** | Not available | Full support |
| **Collection Init** | Via initializer list | Standard C# syntax |

## API Mapping Reference

### Property Access
```csharp
store.Count           // Size of store
store.IsEmpty         // Check if empty  
store.First          // First element
store.Last           // Last element
store.Max()          // Maximum element
store.Min()          // Minimum element
store.MaxMin()       // Returns (max, min) tuple
```

### Element Manipulation
```csharp
store[0]                    // Access without bounds check
store.At(0)                 // Access with bounds check
store[-1]                   // Last element (negative index)
store.PushBack(value)       // Add to end
store.PushFront(value)      // Add to start
store.Insert(pos, value)    // Insert at position
store.RemoveAt(pos)         // Remove at position
store.ReplaceAt(pos, value) // Replace at position
```

### Searching
```csharp
store.Contains(value)                // Check existence
store.CountOf(value)                 // Count occurrences
store.AnyOf(x => x > 10)            // Any matching predicate
store.AllOf(x => x > 0)             // All matching predicate
store.FindAllOf(value)              // Get all positions of value
store.FindAllIf(x => x > 10)        // Get all positions matching predicate
```

### Transformation
```csharp
store.Transform(x => x * 2)         // Modify in-place
store.Filter(x => x > 10)           // Get filtered copy
store.Map(x => x.ToString())        // Project to different type
store.Sort()                        // Sort ascending
store.Unique()                      // Remove duplicates
store.Reverse()                     // Reverse order
```

## Usage Examples

### Basic Operations
```csharp
// Create and populate
var store = new Store<int> { 1, 2, 3, 4, 5 };

// Access
int first = store.First;
int last = store[-1];  // Last element
var slice = store.Slice(1, 4);  // Elements at indices 1,2,3

// Modify
store.PushBack(6);
store.Transform(x => x * 2);
store.Sort();
```

### Filtering and Searching
```csharp
var numbers = new Store<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

// Find positions
var evenPositions = numbers.FindAllIf(x => x % 2 == 0);

// Filter
var evens = numbers.Filter(x => x % 2 == 0);

// Check conditions
bool allPositive = numbers.AllOf(x => x > 0);
bool hasLarge = numbers.AnyOf(x => x > 100);
```

### Type Conversion
```csharp
var strings = new Store<string> { "10", "20", "30" };

// Convert to numbers
var numbers = strings.ToLongLong();
var decimals = numbers.Map(x => (double)x);

// Convert back to string
var result = numbers.ToStr();
Console.WriteLine(result.Join(", "));  // Output: 10, 20, 30
```

### Clone Management
```csharp
var original = new Store<int> { 1, 2, 3 };

// Save state
original.SetClone();

// Modify
original.PushBack(4);

// Restore
original.RestoreClone();  // Back to [1, 2, 3]

// Swap with clone
original.SwapClone();
```

## Error Handling

### Exception Mode (Default)
```csharp
ErrorManager.SetExitCodeMode(false);

try
{
    store.At(100);  // Out of bounds
}
catch (IndexOutOfRangeException ex)
{
    Console.WriteLine(ex.Message);
}
```

### Error Code Mode
```csharp
ErrorManager.SetExitCodeMode(true);

// Operations return error codes instead of throwing
short code = ErrorManager.HandleOutOfRange();
short lastError = ErrorManager.GetLastErrorCode();
```

## Performance Characteristics

- **Append**: O(1) amortized
- **Prepend**: O(n)
- **Insert**: O(n)
- **Remove**: O(n)
- **Search**: O(n)
- **Sort**: O(n log n) using IntroSort
- **Clone**: O(n) - deep copy
- **Slicing**: O(k) where k is slice size (creates new store)

## Thread Safety

- `Store<T>` is **not thread-safe** (standard .NET behavior)
- `ErrorManager` uses `ThreadLocal<T>` for thread-local error state
- For multi-threaded access, use external synchronization:
  ```csharp
  lock (store) { store.PushBack(value); }
  ```

## Requirements

- **.NET 10** or later
- **C# 14.0** or compatible version
- No external dependencies

## Compatibility Notes

- Fully compatible with LINQ operations
- Works with nullable reference types
- Supports both struct and class generic parameters
- Implements standard collection interfaces

## Testing

The `USAGE_EXAMPLES.cs` file contains comprehensive examples demonstrating:
- Basic operations
- Searching and filtering
- Transformation and mapping
- Sorting and deduplication
- Clone management
- Error handling
- Advanced scenarios

All examples are ready to use and can be adapted for specific use cases.

## Future Enhancements

Possible additions:
- `Aggregate()` / `Reduce()` methods
- `ForEach()` with side effects
- `GroupBy()` support
- `Zip()` for combining stores
- Async operations
- Indexer overloads for multiple dimensions
- Observable collection support for WPF/MAUI

## License

Same as original C++ implementation

## Notes

- The C# version simplifies container management by using only `List<T>` internally
- Collection initializer support via `Add()` method implementation
- Full LINQ integration for advanced filtering and transformation
- Thread-local error state for multi-threaded reliability
- Python-style slicing for intuitive element access

# C++ to C# Conversion Summary

## Overview
Successfully converted C++ template-based `store` container class to idiomatic C# `Store<T>` generic class.

## Key Changes

### 1. **Error Handling** (`ErrorManager` Static Class)
- **C++**: Used thread-local error class with both exception and error-code modes
- **C#**: Converted to static class with `ThreadLocal<T>` for thread-safe error tracking
- Methods: `ThrowOutOfRange()`, `ThrowInvalidArgument()`, `ThrowRuntimeError()`
- Error codes: -1 (out of range), -2 (invalid argument), -3 (runtime error)

### 2. **Container Class** (`Store<T>`)

#### From Template to Generic
```csharp
// C++: template<typename T, typename Container = std::vector<T>>
// C#: public class Store<T> : IEnumerable<T>, IEquatable<Store<T>>
```

#### Constructor Simplification
| C++ | C# |
|-----|-----|
| `store()` | `public Store()` |
| `store(size_t size)` | `public Store(int capacity)` |
| `store(size_t size, const T& value)` | `public Store(int size, T value)` |
| `store(initializer_list<T> list)` | `public Store(IEnumerable<T> enumerable)` |

### 3. **Element Access**

| C++ | C# | Notes |
|-----|-----|-------|
| `at(pos)` | `At(pos)` or `this[pos]` | With bounds checking |
| `operator[]` | `this[pos]` | Without bounds checking |
| `front()` | `First` property | Throws if empty |
| `back()` | `Last` property | Throws if empty |
| `max()` | `Max()` method | Uses LINQ |
| `min()` | `Min()` method | Uses LINQ |
| `max_min()` | `MaxMin()` method | Returns tuple |

**Negative Index Support**: Supports Python-style negative indexing (e.g., `-1` for last element)

### 4. **Size & Capacity**

| C++ | C# |
|-----|-----|
| `size()` | `Count` property |
| `empty()` | `IsEmpty` property |
| `reserve(size)` | `Reserve(capacity)` |
| `resize(size)` | `Resize(newSize)` |
| `resize(size, value)` | `Resize(newSize, value)` |
| `clear()` | `Clear()` |

### 5. **Adding/Removing Elements**

| C++ | C# |
|-----|-----|
| `push_front(value)` | `PushFront(value)` |
| `push_back(value)` | `PushBack(value)` |
| `pop_front()` | `PopFront()` |
| `pop_back()` | `PopBack()` |
| `insert(pos, value)` | `Insert(pos, value)` |
| `remove_at(pos)` | `RemoveAt(pos)` |
| `replace_at(pos, value)` | `ReplaceAt(pos, value)` |
| `replace_all(old, new)` | `ReplaceAll(oldValue, newValue)` |
| `fill(value)` | `Fill(value)` |

### 6. **Searching & Filtering**

| C++ | C# |
|-----|-----|
| `contains(value)` | `Contains(value)` |
| `count(value)` | `CountOf(value)` |
| `count_if(predicate)` | `CountIf(predicate)` |
| `any_of(predicate)` | `AnyOf(predicate)` |
| `all_of(predicate)` | `AllOf(predicate)` |
| `none_of(predicate)` | `NoneOf(predicate)` |
| `find_all_of(value)` | `FindAllOf(value)` |
| `find_all_if(predicate)` | `FindAllIf(predicate)` |

### 7. **Transformation & Filtering**

| C++ | C# |
|-----|-----|
| `transform(func)` | `Transform(func)` |
| `filter(predicate)` | `Filter(predicate)` returns new `Store<T>` |
| N/A | `Map<TResult>(selector)` - New method for projection |

### 8. **Sorting & Ordering**

| C++ | C# |
|-----|-----|
| `sort(ascending=true)` | `Sort(ascending=true)` |
| `sort(comparator)` | `Sort(comparer)` or `Sort(comparison)` |
| `unique(autoSort=true)` | `Unique(autoSort=true)` |

### 9. **String Operations**

| C++ | C# |
|-----|-----|
| `join(separator)` | `Join(separator)` |
| `join(separator, converter)` | `Join(separator, converter)` |

### 10. **Slicing/Indexing**

| C++ | C# |
|-----|-----|
| `operator()(start, end, step)` | `Slice(start, end, step)` |
| `operator()(start, end)` | `Slice(start, end)` |

### 11. **Type Conversion**

| C++ | C# |
|-----|-----|
| `to_ll()` | `ToLongLong()` |
| `to_double()` | `ToDouble()` |
| `to_str()` | `ToStr()` |
| N/A | `ToList()` - Convert to `List<T>` |
| N/A | `ToArray()` - Convert to `T[]` |

### 12. **Clone Management**

| C++ | C# |
|-----|-----|
| `set_clone()` | `SetClone()` |
| `clone()` | `GetClone()` |
| `delete_clone()` | `DeleteClone()` |
| `swap_clone()` | `SwapClone()` |
| `restore_clone()` | `RestoreClone()` |

### 13. **Comparison Operators**

| C++ | C# |
|-----|-----|
| `operator<` | `operator <` |
| `operator>` | `operator >` |
| `operator<=` | `operator <=` |
| `operator>=` | `operator >=` |
| `operator==` | `operator ==` & `Equals()` |
| `operator!=` | `operator !=` |

### 14. **Range Generation**

| C++ | C# |
|-----|-----|
| `store::range(start, stop, step)` | `Store<T>.Range(start, stop, step)` |
| `store::range(stop)` | `Store<T>.Range(stop)` |

**Note**: C# version only supports `long` type for range generation

## Usage Examples

### Basic Operations
```csharp
// Create and populate
var store = new Store<int>(5, 10);  // 5 elements with value 10
store.PushBack(20);
store.PushFront(5);

// Access elements
int value = store[0];               // First element (no bounds check)
int last = store[-1];               // Last element (supports negative index)
int checked_value = store.At(1);    // With bounds checking

// Size operations
int count = store.Count;
bool empty = store.IsEmpty;
store.Resize(10);
```

### Searching
```csharp
// Count and search
int occurrences = store.CountOf(20);
bool has20 = store.Contains(20);
var positions = store.FindAllOf(20);

// Predicate-based
int bigNumbers = store.CountIf(x => x > 15);
bool allPositive = store.AllOf(x => x > 0);
var evens = store.FindAllIf(x => x % 2 == 0);
```

### Transformation
```csharp
// Filter and transform
var filtered = store.Filter(x => x > 10);
var doubled = store.Map(x => x * 2);

// In-place transformation
store.Transform(x => x * 2);
store.Sort(ascending: false);
store.Unique(autoSort: true);
```

### Slicing
```csharp
// Python-style slicing
var slice1 = store.Slice(1, 4);        // Elements 1 to 3
var slice2 = store.Slice(0, -1);       // All except last
var slice3 = store.Slice(0, store.Count, 2);  // Every 2nd element
```

### Clone Management
```csharp
store.SetClone();          // Save current state
store.PushBack(100);       // Modify store
store.RestoreClone();      // Restore to saved state
store.SwapClone();         // Swap with clone
```

### Error Handling
```csharp
// Mode 1: Exceptions (default)
ErrorManager.SetExitCodeMode(false);
try
{
    store.At(100);  // Out of bounds
}
catch (IndexOutOfRangeException ex)
{
    Console.WriteLine(ex.Message);
}

// Mode 2: Error codes
ErrorManager.SetExitCodeMode(true);
var code = ErrorManager.HandleOutOfRange();  // Returns -1
short lastError = ErrorManager.GetLastErrorCode();
```

## Key Differences from C++ Version

1. **No Template Container Parameter**: C# version uses `List<T>` internally (no deque/list support at interface level)
2. **LINQ Integration**: Uses LINQ for operations like `Max()`, `Min()`, `Distinct()`, etc.
3. **Property-based Interface**: Uses C# properties (`Count`, `First`, `Last`, `IsEmpty`) where appropriate
4. **IEnumerable Implementation**: Store is fully enumerable with `foreach` support
5. **Thread-Safety**: Error manager uses `ThreadLocal<T>` for thread-safe operation
6. **Simplified Error Modes**: Static error manager instead of instance-based
7. **No Manual Memory Management**: No equivalent to `std::unique_ptr` for clone
8. **Negative Index Normalization**: Normalized at access time, not at storage

## Performance Considerations

- **Internal Storage**: Uses `List<T>` which has O(1) amortized append but O(n) prepend
- **Slicing**: Creates new `Store<T>` instance, no view semantics
- **Sorting**: Uses native `List<T>.Sort()` for efficiency
- **Cloning**: Deep copy via list cloning
- **LINQ Operations**: Lazy evaluation where possible for memory efficiency

## Thread Safety

- `ErrorManager` uses thread-local storage for error state
- Store itself is not thread-safe (standard .NET behavior)
- Use `lock` statements or `ConcurrentBag<T>` for multi-threaded access

## Compatibility Notes

- Target: .NET 10 (C# 14.0)
- No external dependencies
- Fully compatible with .NET Standard patterns
- Supports both nullable and non-nullable reference types

