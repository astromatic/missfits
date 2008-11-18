<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE xsl:stylesheet [
        <!ENTITY nbsp "&#160;">
        <!ENTITY deg "&#176;">
        <!ENTITY amin "&#180;">
        <!ENTITY asec "&#168;">
        <!ENTITY copy "&#169;">
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
     <!-- @import url(http://terapix.iap.fr/terapix.css); -->
     p.sansserif {font-family: sans-serif}
     body {background-color: white}
     mono {font-family: monospace}
     elen {font-family: monospace; font-size: 100%; font-weight: bold; color: green }
     elep {font-family: monospace; font-size: 100%; font-weight: bold; color: red }
     el {font-family: monospace; font-size: 100%; color: black}
     a {text-decoration : none}

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
           <a href="http://terapix.iap.fr"><IMG SRC="http://terapix.iap.fr/cplt/xsl/terapixLogo.png" ALT="Terapix" title="Terapix" border="0"/></a>
          </TD>
          <TD ALIGN="CENTER">
           <a href="http://terapix.iap.fr"><IMG SRC="http://terapix.iap.fr/cplt/xsl/terapixTitle.png" ALT="Logo" title="Terapix" border="0"/></a>
          </TD>
          <TD ALIGN="CENTER" width="2000">
           <FONT color="#669933">
            <B> Processing summary</B>
           </FONT>
          </TD>
          <TD ALIGN="CENTER">
           <IMG SRC="http://terapix.iap.fr/cplt/xsl/terapixPicture.gif" title="Star Formation Region IC 1396, &copy; 2002 CFHT" alt="Star Formation Region IC 1396, &copy; 2002 CFHT"/>
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
     Summary Table on <xsl:value-of select="PARAM[@name='NImages']/@value"/> Output Files
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
      <xsl:for-each select="FIELD[position()>$hflag]">
       <TH BGCOLOR="#FFEECC"><xsl:value-of select="@name"/></TH>
      </xsl:for-each>
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
        <xsl:for-each select="TD[position()>$hflag]">
         <td align="center" BGCOLOR="#EEEEEE">
          <el><xsl:value-of select="self::TD"/></el>
         </td>
        </xsl:for-each>
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
     <xsl:for-each select="PARAM[position()>2]">
      <tr BGCOLOR="#EEEEEE">
       <td><el><xsl:value-of select="@name"/></el></td>
       <td><el><xsl:value-of select="@value"/></el></td>
      </tr>
     </xsl:for-each>
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
