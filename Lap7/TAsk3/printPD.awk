#! /usr/bin/awk -f
BEGIN{FS=","

}
{
    if(NR!=1&&(index($0,"spring")!=0))
        print $0
}