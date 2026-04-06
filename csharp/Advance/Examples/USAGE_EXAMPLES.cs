// C# Store<T> Usage Examples
// Demonstrating the converted class functionality

using System;
using System.Collections.Generic;
using adv;

public class StoreExamples
{
    public static void Main()
    {
        BasicUsageExample();
        SearchingFilteringExample();
        TransformationExample();
        SortingExample();
        CloneManagementExample();
        ErrorHandlingExample();
    }

    // =======================
    // Basic Usage
    // =======================
    public static void BasicUsageExample()
    {
        Console.WriteLine("=== Basic Usage ===\n");

        // Create store
        var numbers = new Store<int>(3, 10);
        numbers.PushBack(20);
        numbers.PushBack(30);
        
        Console.WriteLine($"Store: {numbers}");
        Console.WriteLine($"Count: {numbers.Count}");
        Console.WriteLine($"First: {numbers.First}");
        Console.WriteLine($"Last: {numbers.Last}");

        // Negative indexing
        Console.WriteLine($"Last element [-1]: {numbers[-1]}");
        Console.WriteLine($"Second-to-last [-2]: {numbers[-2]}");

        // Capacity management
        numbers.Reserve(10);
        numbers.Resize(5);
        Console.WriteLine($"After resize to 5: {string.Join(",", numbers.ToArray())}");

        numbers.Clear();
        Console.WriteLine($"After clear, IsEmpty: {numbers.IsEmpty}\n");
    }

    // =======================
    // Searching & Filtering
    // =======================
    public static void SearchingFilteringExample()
    {
        Console.WriteLine("=== Searching & Filtering ===\n");

        var data = new Store<int>();
        int[] values = { 5, 10, 3, 10, 7, 10, 2 };
        foreach (var v in values)
            data.PushBack(v);

        // Searching
        Console.WriteLine($"Contains 10: {data.Contains(10)}");
        Console.WriteLine($"Count of 10: {data.CountOf(10)}");
        Console.WriteLine($"Positions of 10: {string.Join(",", data.FindAllOf(10))}");

        // Conditional searches
        var positions = data.FindAllIf(x => x > 5);
        Console.WriteLine($"Positions where value > 5: {string.Join(",", positions)}");

        var bigNumbers = data.Filter(x => x > 5);
        Console.WriteLine($"Filtered (>5): {string.Join(",", bigNumbers.ToArray())}");

        // Predicate testing
        Console.WriteLine($"All > 0: {data.AllOf(x => x > 0)}");
        Console.WriteLine($"Any = 100: {data.AnyOf(x => x == 100)}");
        Console.WriteLine($"None = 100: {data.NoneOf(x => x == 100)}\n");
    }

    // =======================
    // Transformation
    // =======================
    public static void TransformationExample()
    {
        Console.WriteLine("=== Transformation ===\n");

        var numbers = new Store<int> { 1, 2, 3, 4, 5 };

        // In-place transformation
        numbers.Transform(x => x * 2);
        Console.WriteLine($"After x*2: {string.Join(",", numbers.ToArray())}");

        // Create new with map
        var store = new Store<int> { 1, 2, 3, 4, 5 };
        var doubled = store.Map(x => x * 2);
        Console.WriteLine($"Doubled: {string.Join(",", doubled.ToArray())}");

        // String conversion
        var stringStore = store.ToStr();
        Console.WriteLine($"As strings: {string.Join(",", stringStore.ToArray())}\n");
    }

    // =======================
    // Sorting & Unique
    // =======================
    public static void SortingExample()
    {
        Console.WriteLine("=== Sorting ===\n");

        var unsorted = new Store<int> { 5, 2, 8, 1, 9, 2, 5 };
        Console.WriteLine($"Original: {string.Join(",", unsorted.ToArray())}");

        // Ascending sort
        unsorted.Sort(ascending: true);
        Console.WriteLine($"Sorted ascending: {string.Join(",", unsorted.ToArray())}");

        // Reset and sort descending
        var numbers = new Store<int> { 5, 2, 8, 1, 9, 2, 5 };
        numbers.Sort(ascending: false);
        Console.WriteLine($"Sorted descending: {string.Join(",", numbers.ToArray())}");

        // Remove duplicates
        var withDupes = new Store<int> { 1, 2, 2, 3, 3, 3, 4 };
        withDupes.Unique(autoSort: false);  // Already sorted
        Console.WriteLine($"After unique: {string.Join(",", withDupes.ToArray())}\n");
    }

    // =======================
    // Clone Management
    // =======================
    public static void CloneManagementExample()
    {
        Console.WriteLine("=== Clone Management ===\n");

        var original = new Store<int> { 1, 2, 3 };
        Console.WriteLine($"Original: {string.Join(",", original.ToArray())}");

        // Save state
        original.SetClone();
        Console.WriteLine($"Clone saved");

        // Modify original
        original.PushBack(4);
        original.PushBack(5);
        Console.WriteLine($"After modifications: {string.Join(",", original.ToArray())}");

        // Restore from clone
        original.RestoreClone();
        Console.WriteLine($"After restore: {string.Join(",", original.ToArray())}");

        // Swap with clone
        var fresh = new Store<int> { 10, 20, 30 };
        fresh.SetClone();
        fresh.PushBack(40);
        fresh.SwapClone();
        Console.WriteLine($"After swap: {string.Join(",", fresh.ToArray())}\n");
    }

    // =======================
    // Error Handling
    // =======================
    public static void ErrorHandlingExample()
    {
        Console.WriteLine("=== Error Handling ===\n");

        var store = new Store<int> { 1, 2, 3 };

        // Mode 1: Exception-based (default)
        ErrorManager.SetExitCodeMode(false);
        try
        {
            int value = store.At(10);  // Out of bounds
        }
        catch (IndexOutOfRangeException ex)
        {
            Console.WriteLine($"Caught exception: {ex.Message}");
        }

        // Mode 2: Error codes
        ErrorManager.SetExitCodeMode(false);
        try
        {
            store.At(-10);
        }
        catch (IndexOutOfRangeException)
        {
            short lastError = ErrorManager.GetLastErrorCode();
            Console.WriteLine($"Last error code: {lastError}");
        }

        Console.WriteLine($"Current error mode: {ErrorManager.GetExitCodeMode()}\n");
    }
}

// =======================
// Advanced Examples
// =======================
public class AdvancedExamples
{
    // Working with complex types
    public static void ComplexTypeExample()
    {
        var persons = new Store<Person>
        {
            new Person("Alice", 30),
            new Person("Bob", 25),
            new Person("Charlie", 35)
        };

        // Filter by age
        var adults = persons.Filter(p => p.Age > 25);
        Console.WriteLine($"Adults over 25: {string.Join(",", 
            adults.Map(p => p.Name).ToArray())}");

        // Sort by age
        persons.Sort((p1, p2) => p1.Age.CompareTo(p2.Age));

        // Get names
        var names = persons.Map(p => p.Name);
        Console.WriteLine($"Names: {names.Join(", ")}");
    }

    // Python-style slicing
    public static void SlicingExample()
    {
        var data = new Store<int>();
        for (int i = 0; i < 10; i++)
            data.PushBack(i);

        var slice1 = data.Slice(2, 5);          // [2, 3, 4]
        var slice2 = data.Slice(0, 10, 2);      // [0, 2, 4, 6, 8]
        var slice3 = data.Slice(-3, -1);        // [7, 8]
        
        Console.WriteLine($"Slice [2:5]: {string.Join(",", slice1.ToArray())}");
        Console.WriteLine($"Slice [0:10:2]: {string.Join(",", slice2.ToArray())}");
        Console.WriteLine($"Slice [-3:-1]: {string.Join(",", slice3.ToArray())}");
    }

    // Type conversion pipeline
    public static void TypeConversionExample()
    {
        var stringStore = new Store<string> { "10", "20", "30" };
        
        // Convert to numbers
        var longStore = stringStore.ToLongLong();
        var doubleStore = longStore.Map(x => (double)x);

        Console.WriteLine($"Original: {stringStore.Join(", ")}");
        Console.WriteLine($"As long: {string.Join(",", longStore.ToArray())}");
        Console.WriteLine($"As double: {string.Join(",", doubleStore.ToArray())}");
    }

    // Chaining operations
    public static void ChainingExample()
    {
        var result = new Store<int> { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }
            .Filter(x => x % 2 == 0)           // Keep evens
            .Map(x => x * x);                  // Square them

        Console.WriteLine($"Evens squared: {string.Join(",", result.ToArray())}");
    }

    // Group and aggregate operations
    public static void AggregateExample()
    {
        var numbers = new Store<int> { 5, 2, 8, 1, 9, 3, 7 };

        Console.WriteLine($"Count: {numbers.Count}");
        Console.WriteLine($"Max: {numbers.Max()}");
        Console.WriteLine($"Min: {numbers.Min()}");
        var (max, min) = numbers.MaxMin();
        Console.WriteLine($"Max-Min pair: ({max}, {min})");
        Console.WriteLine($"Sum: {0}");  // Would need Sum() extension
        Console.WriteLine($"Average: {0}");  // Would need Average() extension
    }
}

// Helper class for examples
public class Person
{
    public string Name { get; set; }
    public int Age { get; set; }

    public Person(string name, int age)
    {
        Name = name;
        Age = age;
    }

    public override string ToString() => $"{Name} ({Age})";
}
