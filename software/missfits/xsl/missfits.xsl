<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE xsl:stylesheet [
        <!ENTITY nbsp "&#160;">
        <!ENTITY deg "&#176;">
        <!ENTITY amin "&#180;">
        <!ENTITY asec "&#168;">
        ]>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<!-- *********************** Global XSL template ************************** -->
 <xsl:template match="/">
  <xsl:variable name="date" select="/VOTABLE/RESOURCE/RESOURCE[@name='MetaData']/PARAM[@name='Date']/@value"/>
  <xsl:variable name="time" select="/VOTABLE/RESOURCE/RESOURCE[@name='MetaData']/PARAM[@name='Time']/@value"/>
  <html>

<!-- HTML head  -->

   <head>

<!-- javascript -->

<!--  <script type="text/javascript" language="javascript"> -->

    <script src="http://terapix.iap.fr/cplt/xsl/sorttable.js"/>

    <style type="text/css">
     p.sansserif {font-family: sans-serif}
     body {background-color: white}
     mono {font-family: monospace}
     elen {font-family: monospace; font-size: 100%; font-weight: bold; color: green }
     elep {font-family: monospace; font-size: 100%; font-weight: bold; color: red }
     el {font-family: monospace; font-size: 100%; color: black}
     a {text-decoration: none}
     table.sortable a.sortheader
      {
      background-color:#FFEECC;
      color: black;
      font-weight: bold;
      font-size: 80%;
      text-decoration: none;
      display: button;
      }
     table.sortable span.sortarrow
      {
      color: black;
      font-weight: bold;
      text-decoration: none;
      }
     table.sortable a.sortheader.sub
      {
      vertical-align: sub;
      }
     </style>

     <title>
      Processing summary on <xsl:value-of select="$date"/> at <xsl:value-of select="$time"/>
     </title>
    </head>

<!-- HTML body -->

    <BODY>
     <TABLE BORDER="0" CELLPADDING="0" CELLSPACING="0" WIDTH="100%">
      <TR>
       <TD ALIGN="LEFT">
        <TABLE BORDER="0">
         <TR>
          <TD ALIGN="CENTER">
           <IMG SRC="http://terapix.iap.fr/cplt/xsl/terapixLogo.png" ALT="Terapix"/>
          </TD>
          <TD ALIGN="CENTER">
           <IMG SRC="http://terapix.iap.fr/cplt/xsl/terapixTitle.png" ALT="Logo"/>
          </TD>
          <TD ALIGN="CENTER">
           <FONT color="#669933">
            <B> Processing summary</B>
           </FONT>
          </TD>
          <TD ALIGN="CENTER">
           <IMG SRC="http://terapix.iap.fr/cplt/xsl/terapixPicture.gif" ALT="Terapix banner"/>
          </TD>
         </TR>
        </TABLE>
       </TD>
      </TR>
      <TR>
       <TD>
        <TABLE BORDER="0" WIDTH="100%" BGCOLOR="#000000">
         <TR>
          <TH BGCOLOR="#000000" ALIGN="LEFT"><FONT SIZE="-1" COLOR="#FFFFFF"> Home > Tools > Data reduction</FONT></TH>
         </TR>
        </TABLE>
       </TD>
      </TR>
     </TABLE>
    <xsl:call-template name="VOTable"/>
   </BODY>
  </html>
 </xsl:template>

<!-- **************** Generic XSL template for VOTables ****************** -->
 <xsl:template name="VOTable">
  <xsl:for-each select="/VOTABLE">
   <xsl:call-template name="Resource"/>
  </xsl:for-each>
 </xsl:template>

<!-- *************** Generic XSL template for Resources ****************** -->
 <xsl:template name="Resource">
  <xsl:for-each select="RESOURCE">
   <xsl:choose>
    <xsl:when test="@ID='MissFITS'">
     <xsl:call-template name="missfits"/>
    </xsl:when>
   </xsl:choose>
  </xsl:for-each>
 </xsl:template>

<!-- ******************* XSL template for MissFITS *********************** -->
 <xsl:template name="missfits">
  <xsl:for-each select="RESOURCE[@ID='MetaData']">
   <xsl:call-template name="RunInfo"/>
   <xsl:for-each select="TABLE[@ID='OutFields']">
    <xsl:call-template name="outfields"/>
   </xsl:for-each>
   <xsl:for-each select="RESOURCE[@ID='Config']">
    <xsl:call-template name="Config"/>
   </xsl:for-each>
   <xsl:for-each select="TABLE[@ID='Warnings']">
    <xsl:call-template name="Warnings"/>
   </xsl:for-each>
  </xsl:for-each>
 </xsl:template>

<!-- ************* Generic XSL RunInfo template for MetaData ************* -->
 <xsl:template name="RunInfo">
  <p>
<!-- Software name, version, date, time and number of threads -->
   <a>
    <xsl:attribute name="href">
     <xsl:value-of select="PARAM[@name='Soft_URL']/@value"/>
    </xsl:attribute>
    <b>
     <xsl:value-of select="PARAM[@name='Software']/@value"/>&nbsp;<xsl:value-of select="PARAM[@name='Version']/@value"/>
    </b>
   </a>
   started on
   <b><xsl:value-of select="PARAM[@name='Date']/@value"/></b>
   at
   <b><xsl:value-of select="PARAM[@name='Time']/@value"/></b>
   with
   <b><xsl:value-of select="PARAM[@name='NThreads']/@value"/></b>
   thread<xsl:if test="PARAM[@name='NThreads']/@value &gt; 1">s</xsl:if>

<!-- Run time -->
   <xsl:variable name="duration" select="PARAM[@name='Duration']/@value"/>
   (run time:
    <b>
     <xsl:choose> 
      <xsl:when test="$duration &gt; 3600.0">
       <xsl:value-of
        select='concat(string(floor($duration div 3600)),
        " h ", format-number(floor(($duration div 60) mod 60.0), "00"),
        " min")'/>
      </xsl:when>
      <xsl:otherwise>
       <xsl:choose>
        <xsl:when test="$duration &gt; 60.0">
         <xsl:value-of
          select='concat(format-number(floor($duration div 60),"##"),
          " min ", format-number(floor($duration mod 60.0), "00")," s")'/>
        </xsl:when>
        <xsl:otherwise>
         <xsl:value-of select='concat(string($duration), " s")'/>
        </xsl:otherwise>
       </xsl:choose>
      </xsl:otherwise>
     </xsl:choose>
    </b>)
    <br />
   by user <b><xsl:value-of select="PARAM[@name='User']/@value"/></b>
   from <b><xsl:value-of select="PARAM[@name='Host']/@value"/></b>
   in <b><mono><xsl:value-of select="PARAM[@name='Path']/@value"/></mono></b>
  </p>
  <p>
   <b style="color: red"><xsl:if test="PARAM[@name='Error_Msg']/@value &gt; 0">
    An Error occured!!! </xsl:if>
   <xsl:value-of select="PARAM[@name='Error_Msg']/@value"/></b>
  </p>
  <p>
  <sans-serif><i>click to expand or hide tables</i></sans-serif>
  </p>
 </xsl:template>

<!-- ********************* XSL template for OutFields ******************** -->
  <xsl:template name="outfields">
   <xsl:variable name="imname" select="count(FIELD[@name='Output_Image_Name']/preceding-sibling::FIELD)+1"/>
   <xsl:variable name="inimtype" select="count(FIELD[@name='Input_Image_Type']/preceding-sibling::FIELD)+1"/>
   <xsl:variable name="inext" select="count(FIELD[@name='Input_Extensions']/preceding-sibling::FIELD)+1"/>
   <xsl:variable name="innaxis3" select="count(FIELD[@name='Input_Naxis3']/preceding-sibling::FIELD)+1"/>
   <xsl:variable name="outimtype" select="count(FIELD[@name='Output_Image_Type']/preceding-sibling::FIELD)+1"/>
   <xsl:variable name="outext" select="count(FIELD[@name='Output_Extensions']/preceding-sibling::FIELD)+1"/>
   <xsl:variable name="outnaxis3" select="count(FIELD[@name='Output_Naxis3']/preceding-sibling::FIELD)+1"/>
   <xsl:variable name="hflag" select="count(FIELD[@name='HeadFlag']/preceding-sibling::FIELD)+1"/>
   <p>
    <BUTTON type="button" style="background:#CCEECC; font-family: sans-serif; font-weight: bold;" onclick="showhideTable('missout')">
     Summary Table on Output Files
    </BUTTON>
    <TABLE class="sortable" id="missout" BORDER="2" style="display: none">
     <TR>
      <TH BGCOLOR="#FFEECC">Output Image Name</TH>
      <TH BGCOLOR="#FFEECC">Input Image Type</TH>
      <TH BGCOLOR="#FFEECC">Output Image Type</TH>
      <TH BGCOLOR="#FFEECC">Input Extensions</TH>
      <TH BGCOLOR="#FFEECC">Output Extensions</TH>
      <TH BGCOLOR="#FFEECC">Input Naxis3</TH>
      <TH BGCOLOR="#FFEECC">Output Naxis3</TH>
      <TH BGCOLOR="#FFEECC">External Header</TH>
     </TR>
     <xsl:for-each select="DATA/TABLEDATA">
      <xsl:for-each select="TR">
       <tr>
        <td align="left" BGCOLOR="#EEEEEE">
         <el><xsl:value-of select="TD[$imname]"/></el>
        </td>
        <td align="center" BGCOLOR="#EEEEEE">
         <elen><xsl:value-of select="TD[$inimtype]"/></elen>
        </td>
        <td align="center" BGCOLOR="#EEEEEE">
         <elen><xsl:value-of select="TD[$outimtype]"/></elen>
        </td>
        <td align="center" BGCOLOR="#EEEEEE">
         <el><xsl:value-of select="TD[$inext]"/></el>
        </td>
        <td align="center" BGCOLOR="#EEEEEE">
         <el><xsl:value-of select="TD[$outext]"/></el>
        </td>
        <td align="center" BGCOLOR="#EEEEEE">
         <el><xsl:value-of select="TD[$innaxis3]"/></el>
        </td>
        <td align="center" BGCOLOR="#EEEEEE">
         <el><xsl:value-of select="TD[$outnaxis3]"/></el>
        </td>
        <td align="center" BGCOLOR="#EEEEEE">
         <xsl:choose>
         <xsl:when test="TD[$hflag] = 'T'">
           <elen>Y</elen>
          </xsl:when>
          <xsl:otherwise>
           <elen>N</elen>
          </xsl:otherwise>
         </xsl:choose>
        </td>
       </tr>
      </xsl:for-each>
     </xsl:for-each>
    </TABLE>
   </p>
 </xsl:template>

<!-- ********************** XSL template for Config File ********************** -->
  <xsl:template name="Config">
   <p>
    <BUTTON type="button" style="background:#CCEECC; font-family: sans-serif; font-weight: bold;" onclick="showhideTable('config')">
     Configuration File: <xsl:value-of select="PARAM[@name='Prefs_Name']/@value"/>
    </BUTTON>
    <TABLE id="config" class="sortable" style="display: none">
     <TR>
      <TH BGCOLOR="#FFEECC">Config Parameter</TH>
      <TH BGCOLOR="#FFEECC">Value</TH>
     </TR>
     <tr BGCOLOR="#EEEEEE">
      <td><el>REMOVE_KEYWORD</el></td>
      <td><el><xsl:value-of select="PARAM[@name='Remove_Keyword']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>REPLACE_KEYWORD</el></td>
      <td><el><xsl:value-of select="PARAM[@name='Replace_Keyword']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>SLICE_KEYWORD</el></td>
      <td><el><xsl:value-of select="PARAM[@name='Slice_Keyword']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>SLICEKEY_FORMAT</el></td>
      <td><el><xsl:value-of select="PARAM[@name='SliceKey_Format']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>DISPLAY_KEYWORD</el></td>
      <td><el><xsl:value-of select="PARAM[@name='Display_Keyword']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>HEADER_SUFFIX</el></td>
      <td><el><xsl:value-of select="PARAM[@name='Header_Suffix']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>NEXTENSIONS_MIN</el></td>
      <td><el><xsl:value-of select="PARAM[@name='NExtensions_Min']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>OUTFILE_TYPE</el></td>
      <td><el><xsl:value-of select="PARAM[@name='OutFile_Type']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>SPLIT_SUFFIX</el></td>
      <td><el><xsl:value-of select="PARAM[@name='Split_Suffix']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>SLICE_SUFFIX</el></td>
      <td><el><xsl:value-of select="PARAM[@name='Slice_Suffix']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>PROCESS_TYPE</el></td>
      <td><el><xsl:value-of select="PARAM[@name='Process_Type']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>CHECKSUM_TYPE</el></td>
      <td><el><xsl:value-of select="PARAM[@name='Checksum_Type']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>SAVE_TYPE</el></td>
      <td><el><xsl:value-of select="PARAM[@name='Save_Type']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>NEW_SUFFIX</el></td>
      <td><el><xsl:value-of select="PARAM[@name='New_Suffix']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>VERBOSE_TYPE</el></td>
      <td><el><xsl:value-of select="PARAM[@name='Verbose_Type']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>WRITE_XML</el></td>
      <td><el><xsl:value-of select="PARAM[@name='Write_XML']/@value"/></el></td>
     </tr>
     <tr BGCOLOR="#EEEEEE">
      <td><el>NTHREADS</el></td>
      <td><el><xsl:value-of select="PARAM[@name='NThreads']/@value"/></el></td>
     </tr>
    </TABLE>
   </p>
   <p>
    <BUTTON type="button" style="background:#CCEECC; font-family: monospace; font-weight: bold: font-size: 80%;" onclick="showhideTable('commandline')">
     Command Line
    </BUTTON>
    <TABLE id="commandline" style="display: none">
     <TR>
      <TD BGCOLOR="#FFEECC" style="font-size: 80%;"><el>Command Line: <xsl:value-of select="PARAM[@name='Command_Line']/@value"/></el></TD>
     </TR>
    </TABLE>
   </p>
  </xsl:template>

<!-- ********************** XSL template for Warnings ********************** -->
  <xsl:template name="Warnings">
   <xsl:variable name="date" select="count(FIELD[@name='Date']/preceding-sibling::FIELD)+1"/>
   <xsl:variable name="time" select="count(FIELD[@name='Time']/preceding-sibling::FIELD)+1"/>
   <xsl:variable name="msg" select="count(FIELD[@name='Msg']/preceding-sibling::FIELD)+1"/>
   <p>
    <BUTTON type="button" style="background:#CCEECC; font-family: monospace; font-weight: bold: font-size: 80%;" onclick="showhideTable('warnings')">
     Warnings (limited to the last 100)
    </BUTTON>
    <TABLE id="warnings" style="display: none">
     <TR style="font-size: 80%;">
      <TH BGCOLOR="#FFEECC">Date</TH>
      <TH BGCOLOR="#FFEECC">Time</TH>
      <TH BGCOLOR="#FFEECC">Message</TH>
     </TR>
     <xsl:for-each select="DATA/TABLEDATA">
      <xsl:for-each select="TR">
       <tr>
        <td  BGCOLOR="#EEEEEE">
         <el><xsl:value-of select="TD[$date]"/></el>
        </td>
        <td BGCOLOR="#EEEEEE">
         <el><xsl:value-of select="TD[$time]"/></el>
        </td>
        <td align="center" BGCOLOR="#EEEEEE">
         <el><xsl:value-of select="TD[$msg]"/></el>
        </td>
       </tr>
      </xsl:for-each>
     </xsl:for-each>
    </TABLE>
   </p>
 </xsl:template>

 <xsl:template name="Rest">
</xsl:template>
</xsl:stylesheet>
