// https://www.geeksforgeeks.org/implementing-our-own-hash-table-with-separate-chaining-in-java/
internal class HashNode<K, V>
(var key: K, var value: V) {
    var next: HashNode<K, V>? = null
}

internal class Map<K, V> {
    private var bucketArray: java.util.ArrayList<HashNode<K, V>?>

    private var numBuckets: Int

    private var size: Int
    fun size(): Int {
        return size
    }

    val isEmpty: Boolean
        get() = size() == 0

    private fun getBucketIndex(key: K): Int {
        val hashCode = key!!.hashCode()
        var index = hashCode % numBuckets
        index = if (index < 0) index * -1 else index
        return index
    }

    fun remove(key: K): V? {
        val bucketIndex = getBucketIndex(key)

        var head: HashNode<K, V>? = bucketArray.get(bucketIndex)

        var prev: HashNode<K, V>? = null
        while (head != null) {
            if (head.key == key) break

            prev = head
            head = head.next
        }

        if (head == null) return null

        size--

        if (prev != null) prev.next = head.next else bucketArray.set(bucketIndex, head.next)
        return head.value
    }

    operator fun get(key: K): V? {
        val bucketIndex = getBucketIndex(key)
        var head: HashNode<K, V>? = bucketArray.get(bucketIndex)

        while (head != null) {
            if (head.key == key) return head.value
            head = head.next
        }

        return null
    }

    fun add(key: K, value: V) {
        val bucketIndex = getBucketIndex(key)
        var head: HashNode<K, V>? = bucketArray.get(bucketIndex)

        while (head != null) {
            if (head.key == key) {
                head.value = value
                return
            }
            head = head.next
        }

        size++
        head = bucketArray.get(bucketIndex)
        val newNode = HashNode(key, value)
        newNode.next = head
        bucketArray.set(bucketIndex, newNode)

        if (1.0 * size / numBuckets >= 0.7) {
            val temp: java.util.ArrayList<HashNode<K, V>?> = bucketArray
            bucketArray = java.util.ArrayList<HashNode<K, V>?>()
            numBuckets = 2 * numBuckets
            size = 0
            for (i in 0 until numBuckets) bucketArray.add(null)
            var headNode: HashNode<K, V>? = temp[0]
            for (i in 0..temp.size) {
                while (headNode != null) {
                    add(headNode.key, headNode.value)
                    headNode = headNode.next
                }
            }
        }
    }

    init {
        bucketArray = java.util.ArrayList<HashNode<K, V>?>()
        numBuckets = 10
        size = 0

        for (i in 0 until numBuckets) bucketArray.add(null)
    }
}

fun main() {
    val map = Map<String, String>()
    map.add("owo", "data")
    println(map.get("owo"))
}