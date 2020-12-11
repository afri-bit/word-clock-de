
/**
 * @brief Function to convert bytes to integer number
 * 
 * @param bytearray Pointer to byte array
 * @param size Size of the array
 * @return int Integer number from the bytes
 */
int bytes_to_int(uint8_t *bytearray, int size)
{
    int ret = 0;
    if (size > 4)
    {
        return 0;
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            int tmp = bytearray[i] << i * 8;
            ret = ret + tmp;
        }
    }

    return ret;
}