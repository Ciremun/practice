const TEST_ARR_SIZE: usize = 69;

fn partition(arr: &mut [i32], low: usize, high: usize) -> i32
{
    let pivot: i32 = arr[high];
    let mut i: i32 = (low as i32) - 1;
    for j in low..high
    {
        if arr[j] < pivot
        {
            i += 1;
            arr.swap(i as usize, j);
        }
    }
    arr.swap((i + 1) as usize, high);
    return i + 1;
}
 
fn quick_sort(arr: &mut [i32], low: i32, high: i32)
{
    if low < high
    {
        let pi: i32 = partition(arr, low as usize, high as usize);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi  + 1, high);
    }
}

fn main()
{
    let mut arr: [i32; TEST_ARR_SIZE] = [0; TEST_ARR_SIZE];
    for i in 0..TEST_ARR_SIZE { if i % 2 == 0 { arr[i] = (i as i32) + 1; } else { arr[i] = (i as i32) - 1; } }
    println!("{:?}\n->", arr);
    quick_sort(&mut arr, 0, (TEST_ARR_SIZE - 1) as i32);
    println!("{:?}", arr);
}
