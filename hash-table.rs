struct HashNode<K, V>
{
    key: K,
    value: V
}

struct HashMap<K, V>
{
    nodes: Option<Vec<HashNode<K, V>>>,
    capacity: usize,
    size: usize,
    dummy: HashNode<K, V>
}

fn main()
{
    let dummy = HashNode {key: -1, value: -1};
    let map = HashMap {nodes: None, capacity: 20, size: 0, dummy: dummy};
    println!("map.capacity: {}", map.capacity);
}