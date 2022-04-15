@rem ** WARNING:: DEMQ has patched DSRLNK that does not DSRLNK!
@rem using tiobj2bin on unpack will leave a big header you need to manually delete
@rem remember to take the c99 E/A startup code out of the executable, it loads in
@rem at startup and we need to delete the E/A copy code. At >A00C you should see
@rem bytes 0201 20FA 0202 02C2 - these load R1 with the destination address, and R2
@rem with the length. Change 20FA to 0000 (write to ROM) and 02C2 to 0002 (two bytes)
@rem this code loads color to a lower address then moves it up, delete the move code
@rem There are two in this code: 

\work\setbinsize\release\setbinsize.exe header.bin 64
\work\setbinsize\release\setbinsize.exe unpack.bin 1024
\work\setbinsize\release\setbinsize.exe watrescue.pack 5210
\work\setbinsize\release\setbinsize.exe watrescuf.pack 3462
\work\setbinsize\release\setbinsize.exe watrescug.pack 4598
\work\setbinsize\release\setbinsize.exe wrendc.pack 1394
\work\setbinsize\release\setbinsize.exe wrendp.pack 3028
\work\setbinsize\release\setbinsize.exe wrtitc.pack 1014
\work\setbinsize\release\setbinsize.exe wrtitp.pack 2558
\work\setbinsize\release\setbinsize.exe demq.pack 1792

@rem those are the only files that are stacked

copy /y /b header.bin + /b unpack.bin + /b watrescue.pack + /b wrendc.pack /b bank0.bin
copy /y /b header.bin + /b watrescuf.pack + /b watrescug.pack /b bank1.bin
copy /y /b header.bin + /b wrendp.pack + /b wrtitc.pack + /b wrtitp.pack /b bank2.bin
copy /y /b header.bin + /b demq.pack + /b text.txt /b bank3.bin

\work\setbinsize\release\setbinsize.exe bank0.bin 8192
\work\setbinsize\release\setbinsize.exe bank1.bin 8192
\work\setbinsize\release\setbinsize.exe bank2.bin 8192
\work\setbinsize\release\setbinsize.exe bank3.bin 8192

copy /y /b bank0.bin + /b bank1.bin + /b bank2.bin + /b bank3.bin /b WatRescue32KB8.bin

