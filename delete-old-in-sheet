function deleteOldteste() {
  var ss = SpreadsheetApp.getActiveSpreadsheet();
  var sheet = ss.getSheets()[0];
  var firstCell=sheet.getSheetValues(2,1,1,1)
  var date = new Date();
  var today = date.toLocaleString('pt-br').split(" ")[0];
  var begin =firstCell.toLocaleString('pt-br').split(" ")[0];
  //compara se o início já não está vazio
  if(begin!=""){
     //compara se a data da primeira célular é igual a data de hoje, considera apenas dia mes e ano
     if (today!=begin){
       var lastRow = sheet.getLastRow();
       var lastCell=sheet.getSheetValues(lastRow,1,1,1);
       var final=lastCell.toLocaleString('pt-br').split(" ")[0];
       //compara se a primeira célula tem a mesma data da última
       //se não tiver acha ultima celula que tem datas iguais
       if (begin!=final){
         //encontra a ultima da sequencia da data
         for(i=2;i<lastRow;i++){
           let cell=sheet.getSheetValues(i,1,1,1);
           let dateCell=cell.toLocaleString('pt-br').split(" ")[0];
           if(dateCell!=begin){
             var dateLastCell=i-1;
             break;
           }
         }
       }else{
         dateLastCell=lastRow;
       }
       columnsToCopy=4
       var toCopy = sheet.getSheetValues(2,1,dateLastCell-1,columnsToCopy);
       //verifica na planilha de arquivos a ultima coluna
       var sheet2= ss.getSheets()[1];
       var histLastRow = sheet2.getLastRow();
       //adiciona mais 1 pra deixar uma linha de folga
       histLastRow=histLastRow+2
       sheet2.getRange(histLastRow,1,dateLastCell-1,columnsToCopy).setValues(toCopy)
       sheet.deleteRows(2,dateLastCell-1)
       
       
    }
     
  }
  
}
