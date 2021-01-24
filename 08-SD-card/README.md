<br>

# SD-card Example


This example uses the FAT file system library written by ChaN. The full, very comprehensive documentation can be found here: [FatFs Module Documentation](http://elm-chan.org/fsw/ff/00index_e.html)

To try the example put the files in the **put_in_tf_card** folder in the root directory of the SD-card.

The program will write a text file to disk and then open some images from the card to display a clock.

You can convert images using this useful web-tool: https://lvgl.io/tools/imageconverter
Use RGB656 binary as the output.

## Functions
```c
FRESULT f_mount (
  FATFS*       fs,    /* [IN] Filesystem object */
  const TCHAR* path,  /* [IN] Logical drive number */
  BYTE         opt    /* [IN] Initialization option */
);
```
Use this to mount the filesystem.
```c
FRESULT f_open (
  FIL* fp,           /* [OUT] Pointer to the file object structure */
  const TCHAR* path, /* [IN] File name */
  BYTE mode          /* [IN] Mode flags */
);
```
Opening a file.
```c
FRESULT f_write (
  FIL* fp,          /* [IN] Pointer to the file object structure */
  const void* buff, /* [IN] Pointer to the data to be written */
  UINT btw,         /* [IN] Number of bytes to write */
  UINT* bw          /* [OUT] Pointer to the variable to return number of bytes written */
);
```
Write to file.
```c
FRESULT f_read (
  FIL* fp,     /* [IN] File object */
  void* buff,  /* [OUT] Buffer to store read data */
  UINT btr,    /* [IN] Number of bytes to read */
  UINT* br     /* [OUT] Number of bytes read */
);
```
Read from a file.
```c
FRESULT f_close (
  FIL* fp     /* [IN] Pointer to the file object */
);
```
Close the file. Flushes any unwritten data into the file. If you just want to flush buffered data you can also use f_sync() to flush data but keep the file open.
