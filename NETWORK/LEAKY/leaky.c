/* leaky bucket 
### **Algorithm for Leaky Bucket:**

1. **Initialize Parameters:**  
   - Read `buck_size` (bucket capacity), `outgoing` (outflow rate), and `n` (number of incoming packets).  
   - Set `store = 0` (current buffer occupancy).  

2. **Process Incoming Packets (Loop for `n` packets):**  
   - Read `incoming` (size of the incoming packet).  
   - If `incoming` fits within available buffer space:  
     - Add `incoming` to `store`.  
   - Else:  
     - Drop excess packets.  
     - Fill the bucket to its maximum capacity.  

3. **Simulate Outgoing Packet Transmission:**  
   - Reduce `store` by `outgoing`.  
   - If `store` becomes negative, set it to zero.  

4. **Repeat Until All Packets Are Processed.**  

5. **End.***/

#include <stdio.h>

int main()
{
    int incoming, outgoing, buck_size, n, store = 0;
    
    printf("Enter bucket size, outgoing rate, and number of inputs:\n");
    scanf("%d %d %d", &buck_size, &outgoing, &n);
    
    while(n != 0)
    {
        printf("Enter incoming packet size:\n");
        scanf("%d", &incoming);
        printf("Incoming packet size: %d\n", incoming);
        
        if (incoming <= (buck_size - store)) // Check if packet can fit into the buffer
        {
            store += incoming; // Store the incoming packet in the buffer
            printf("Bucket buffer size %d out of %d\n", store, buck_size);
        }
        else // If incoming packet can't fit, drop the excess
        {
            printf("Dropped %d no. of packets\n", incoming - (buck_size - store));
            store = buck_size; // Bucket is full
            printf("Bucket buffer size %d out of %d\n", store, buck_size);
        }

        // Simulate outgoing packets
        store -= outgoing;
        if (store < 0) // If outgoing rate exceeds the current store, set store to 0
        {
            store = 0;
        }
        
        printf("After outgoing, %d packets left out of %d in buffer\n", store, buck_size);
        n--; // Decrease the number of inputs
    }

    return 0;
}

