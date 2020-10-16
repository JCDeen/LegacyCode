<?php

class RESTApi {

  protected $Verbose = false;
  protected $Url = '';
  protected $ReqType = '';
  protected $Params = null;
  protected $SimpleAuth = null;
  protected $RetTransfer = true;
  
  public function __construct( $RestUrl = "", $RestParams = "", $ReqType = "", $User = "", $Password = "", $RetTransfer = "" ) {
    if( !empty( $RestUrl ) )
    { $this->Url = $RestUrl; }

    if( !empty( $RestParams ) )
    { $this->Params = $RestParams; }
	
    if( $ReqType == "PUT" )
    { $this->ReqType = "PUT"; }
    
    if( !empty( $User ) && !empty( $Password ) )
    { $this->SimpleAuth = $User . ":" . $Password; }
  }

  // Url and Param array can be passed in constructor 
  // or Post construction
  public function getUrl()
  { return $this->Url; }
  public function setUrl( $NewUrl )
  { $this->Url = $NewUrl; }

  public function getFullParams()
  { return $this->Params; }
  public function setFullParams( $NewParams )
  { $this->Params = $NewParams; }

  public function getReqType()
  { return $this->ReqType; }
  public function setReqType( $ReqType )
  { $this->ReqType = $ReqType; }

  public function getRetTransfer()
  { return $this->RetTransfer; }
  public function setRetTransfer( $RetTransfer )
  { $this->RetTransfer = $RetTransfer; }
  
  public function getVerbosity()
  { return $this->Verbose; }

  public function setVerbosity( $Verbosity )
  { $this->Verbose = $Verbosity; }

  public function doRest( $UriExtension = "" )
  {
    if( empty( $this->Url ) )
    { 
      error_log( "RESTApi::doRest requires Url to be set" );
      return false;
    }
    if( !isset( $this->Params ) )
    { 
      error_log( "RESTApi::doRest requires Params to be set" );
      return false;
    }

    $Options = http_build_query( $this->Params );

    $Ch = curl_init( );
    $FullUrl = $this->Url;
    if( $UriExtension != "" )
    { $FullUrl .= "/" . $UriExtension; }
    if( $this->Verbose )
    { print( "URL: ".$FullUrl."?".$Options.PHP_EOL );}
    
    curl_setopt( $Ch, CURLOPT_URL, $FullUrl."?".$Options );
    if( $this->RetTransfer == true )
    { curl_setopt( $Ch, CURLOPT_RETURNTRANSFER, 1 ); }
    if( $this->SimpleAuth )
    { curl_setopt( $Ch, CURLOPT_USERPWD, $SimpleAuth ); }
    
    if( $this->ReqType == "PUT" )
    { curl_setopt($Ch, CURLOPT_CUSTOMREQUEST, "PUT"); }
    else if( $this->ReqType == "GET" )
    { curl_setopt($Ch, CURLOPT_CUSTOMREQUEST, "GET"); }
    if( $this->Verbose )
    { curl_setopt( $Ch, CURLOPT_VERBOSE, true ); }

    $Return = curl_exec( $Ch );

    curl_close( $Ch );
    return $Return;
  }
}
?>
