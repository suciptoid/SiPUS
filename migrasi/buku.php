<?php

mysql_connect("localhost","root","root");
mysql_select_db("libska_qt");

echo "\r\r\r\r\r\r\r\r\n";
//1. First Alter Table

//2. Select all book to count :D
$cari="SELECT kd_buku, no FROM tbl_buku ORDER BY no ";

$sql=mysql_query($cari);

$jumlah=0;
$berhasil=0;
$gagal=0;

//3. Converting Book Code
while($kode=mysql_fetch_array($sql)){
    
    $kd=explode("/",$kode[0]);
        $kd_belakang=$kd[2];
        $split=explode("-",$kd_belakang);
    
    $newcode = $kd[0]."/".$kd[1]."/".$split[0];
    $replace="UPDATE tbl_buku SET kd_buku=\"".$newcode."\" WHERE kd_buku=\"".$kode[0]."\"";
    
    if(mysql_query($replace)){
        echo number_format($kode[1])." Berhasil : ".$kode[0]." => ".$newcode."\r";
        
        //abaikan
        $chek_sql = "SELECT COUNT(*) FROM tbl_buku WHERE kd_buku=\"".$kode[0]."\"";
        $chek_query=mysql_query($chek_sql);
        $chek_result=mysql_fetch_array($chek_query);
        if($chek_result[0]==0){
            //break;
        }
        $berhasil++;
    }else{
        echo $kode[1]." Eroorrr ".$replace."\n\t".mysql_error()."\r";
        $gagal++;
    }
    //echo $replace."<br>";
    
    
    
    //$replace=mysql_query("UPDATE tbl_buku SET kd_buku")
    $jumlah++;
}
echo "\n\n Total \t\t: $jumlah \n Berhasil \t\t: $berhasil \n Gagal \t\t: $gagal\n";

?>
