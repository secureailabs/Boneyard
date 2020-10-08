import React, { Component } from 'react';
import ProgressBar from 'react-bootstrap/ProgressBar'
import { withRouter } from 'react-router-dom';
import { uploadFile } from '/imports/api/uploadFile.js'
import { listEnclaves } from '/imports/api/usercfg.js'
import { roles } from '/imports/api/enum.js'
import { Redirect } from 'react-router'
import { Random } from 'meteor/random'
import { EJSON } from 'meteor/ejson'

import ReactTable from 'react-table';
//import 'react-table/react-table.css';
import { Modal, Container, Row, Col, Button, ButtonToolbar, ButtonGroup, Tooltip, OverlayTrigger, Form, Card } from 'react-bootstrap'

import { DappFormGroup } from '/imports/api/ui/DappFormGroup.js'

import SplitPane from 'react-split-pane';

import 'react-ui-tree/dist/react-ui-tree.css';
import './tree.css';
import cx from 'classnames';
import Tree from 'react-ui-tree';
import tree from './tree';

import { Controlled as CodeMirror } from 'react-codemirror2'
import 'codemirror/lib/codemirror.css';
import 'codemirror/mode/r/r';
import 'codemirror/theme/ambiance.css';

import '/imports/api/splitpane.css';
import './browser.css';

const byPropKey = (propertyName, value) => () => ({
  [propertyName]: value,
});

function setParent(tree){
    if (undefined !== tree.children && tree.children.length>0)
      tree.children.map(child=>{
        child.parentNode=tree;
        setParent(child);
      })
}

function setCollapsed(tree, cnt){
  if (undefined !== tree.children){
    if (cnt>0){
       tree.collapsed = false
       cnt = cnt-1
    } else
       tree.collapsed = true
    tree.children.map(child=>{
      child.parentNode=tree;
      setCollapsed(child, cnt);
    })
  }
}

function treeSort(tree){
   if (undefined === tree)
     return
   if (undefined !== tree.children){
     tree.children.sort(function(a, b){
       return(a.module-b.module)
     })
     tree.children.map((c)=>{
       if (!c.leaf)
         treeSort(c)
     })
   }
}

function  renderTree(tree){
  setParent(tree)
  setCollapsed(tree, 2)
  treeSort(tree)
}

function existChild(treeNode, name){
  if (undefined === treeNode || null === treeNode)
    return false;
  for (i in treeNode.children)
    if (treeNode.children[i].module === name)
      return true
  return false;
}

class BrowseEnclave extends Component {
  constructor(props) {
    super(props);
    this.newTabIndex = 0;
    this.state = {
      active: null,
      activeType: "other",
      panes:[],
      activeKey: 0,
      resultR: '',
      enclaves: [],
      pubKey: '',
      error: '',
      tree: tree,
      //role:'Secure Enclave',
      ethAccounts: [],
      ethAcc: '',
      showUploadData: false,
      uploadFilename:'',
      showNewModel:false,
      newModelName:'',
      modelResult:'outputs',
      activePath:'',
      condition:''
    };

    //this.show = () => this.setState({ show: true });
    //this.handleSelect = this.handleSelect.bind(this);
    this.selectEnclave = this.selectEnclave.bind(this);
    this.selectEthAcc = this.selectEthAcc.bind(this);
    this.uploadData = this.uploadData.bind(this);
    this.uploadLargeData = this.uploadLargeData.bind(this)
    this.handleShowUploadData = this.handleShowUploadData.bind(this);
    this.handleCloseUploadData = this.handleCloseUploadData.bind(this);
    this.handleShowNewModel = this.handleShowNewModel.bind(this);
    this.handleCloseNewModel = this.handleCloseNewModel.bind(this);
    this.newModelFile = this.newModelFile.bind(this);
    this.uploadModel = this.uploadModel.bind(this);
    this.runModel = this.runModel.bind(this);
    this.deleteData = this.deleteData.bind(this);
    this.refreshTreeNode = this.refreshTreeNode.bind(this);
    this.onGetDataStatistics = this.onGetDataStatistics.bind(this);
    this.deleteModel = this.deleteModel.bind(this);
  }

  refreshNode = (node, path, activeType) => {
    if (null === node)
      return;
    const { pubKey, ethAcc} = this.state
    Meteor.call("getEnclaveInfo", pubKey, ethAcc, path, (err, res)=>{
      if (err)
        alert(err.toString())
      else if (res.error)
        alert(res.error)
      else {
        //if (node.module !== res.result.module) {
        //  alert("Get enclave information for "+path+" failed.")
        //  return
        //}
        node.children = res.result.children
        renderTree(node);
        //if (this.state.active === node)
        this.setState({active:node, activePath:path, activeType:activeType})
      }
   })
  }

  refreshTreeNode(event){
    const { active, activePath, activeType } = this.state
    this.refreshNode(active, activePath, activeType)
  }

  deleteData(event) {
    const { active, pubKey, ethAcc, modelResult, activePath } = this.state;
    if (null === active)
      return;

    if (!confirm("Deleting "+activePath+" !!! press OK to continue"))
      return

    Meteor.call("userDelDataFrontEnd", 
      pubKey, ethAcc, activePath, (error, result) => {
        if (error) {
          console.log("error:", error)
          if ("object" === typeof error)
            error = JSON.stringify(error)
          alert(error)
        }
        else if (result.error)
          alert(result.error)
        else {
          console.log("delete data returned.");
          const  pNode = active.parentNode
          if (undefined !== pNode) {
            pNode.children = pNode.children.filter((child)=>(child !== active))
            //this.setState({ active:pNode })
            this.onClickNode(pNode)
          } else
            this.setState({ active:null })
        }
    })
  }

  deleteModel(event) {
    const { active, pubKey, ethAcc, activePath } = this.state;
    if (null === active)
      return;

    if (!confirm("Deleting "+activePath+" !!! press OK to continue"))
      return

    Meteor.call("userDeleteModel", 
      activePath.replace("/model/",""), pubKey, ethAcc, (error, result) => {
        if (error) {
          console.log("error:", error)
          if ("object" === typeof error)
            error = JSON.stringify(error)
          alert(error)
        }
        else if (result.error)
          alert(result.error)
        else {
          console.log("delete data returned.");
          const  pNode = active.parentNode
          if (undefined !== pNode) {
            pNode.children = pNode.children.filter((child)=>(child !== active))
            //this.setState({ active:pNode })
            this.onClickNode(pNode)
          } else
            this.setState({ active:null })
        }
    })
  }

  runModel(event) {
    const { active, pubKey, ethAcc, modelResult, activePath } = this.state;
    console.log("run mddelllll");
    if (null === active || true !== active.leaf || undefined === active.content)
      return;

    Meteor.call("userRunCode", 
      {title:activePath.replace("/model",""), content:active.content},
      pubKey, ethAcc, (error, result) => {
        if (error) {
          console.log("error:", error)
          if ("object" === typeof error)
            error = JSON.stringify(error)
          this.setState({ modelResult: error })
        }
        else if (result.error)
          this.setState({ modelResult: result.error })
        else {
          console.log("Run model returned.");
          active.modified = false
          this.setState({ modelResult: result.result, active:active })
        }
    })
  }

  uploadModel(event){
    const { active, pubKey, ethAcc, modelResult, activePath } = this.state;
    if (null === active || true !== active.leaf || undefined === active.content)
      return;

    Meteor.call("userUploadModel", 
      {title:activePath.replace("/model",""), content:active.content},
      pubKey, ethAcc, (error, result) => {
        if (error) {
          console.log("error:", error)
          if ("object" === typeof error)
            error = JSON.stringify(error)
          this.setState({ modelResult: error })
        }
        else if (result.error)
          this.setState({ modelResult: result.error })
        else {
          console.log("model uploaded");
          active.modified = false
          this.setState({ modelResult: result.result, active:active })
        }
    })
  }

  newModelFile(event){
    var active = this.state.active
    if (undefined === active || null === active) {
      alert("No current active node");
      return;
    }
    if (existChild(active, newModelName)){
      alert("Model exist")
      return;
    }
    if (undefined === active.children) {
      alert("Cannot add new model to a non-folder node")
      return;
    }
    active.children.push({module:this.state.newModelName,leaf:true, parentNode:active, modified:true})
    this.handleCloseNewModel()
  }

  uploadLargeData(file){
    var reader = new FileReader();
    var size = file.size;
    var chunk_size = 4194304;
    var chunks = [];
    var offset = 0;
    var bytes = 0;
    var pubKey = this.state.pubKey
    var ethAcc = this.state.ethAcc
    const {active, activePath} = this.state
    var state = this.state
    var noError = true
    var th = this
    
    document.getElementById("fileProgressBar").removeAttribute("hidden")
    readSlice(file);

  function readSlice(file){
  var reader = new FileReader();
  reader.onload = function(e){
  if (reader.readyState == FileReader.DONE){
    console.log("Uploading the packet");
      Meteor.call("userShareDataFrontEnd",
      pubKey,
      ethAcc,
      activePath.replace("/data","")+'/'+file.name,
      btoa(reader.result), 
      offset, 
      1,(error, result) => {
        if(!error){
          if(result.error !== undefined){
            noError = false
            console.log(result.error);
          } else if(result.receipt !== "0x239492347934875349"){
            noError = false
            console.log(error);
          } else {
            console.log('result ' + (offset+chunk_size)*100/size + '%');
            document.getElementById("fileProgressBar").childNodes[0].style.width = "" + ((offset+chunk_size
              )*100/size).toString(10) + "%"
            document.getElementById("fileProgressBar").childNodes[0].innerHTML = file.name
            if(offset+chunk_size < size && noError === true){
              offset += chunk_size
              readSlice(file)
            } else {
              closeFile()
            }
          }
        } else{
          console.log(error);
        }
      })
    }
  }
  var blob = file.slice(offset, offset + chunk_size);
  reader.readAsBinaryString(blob);
  }
    
    function closeFile(){
      console.log("Uploading the packet");
          Meteor.call("userShareDataFrontEnd",
          pubKey,
          ethAcc,
          activePath.replace("/data","")+'/'+file.name,
          "", 
          -1, 
          1,(error, result) => {
            if(!error){
              console.log('File Upload : success ' + 100 + '%');
              var el = document.getElementById("fileProgressBar").childNodes[0]
              el.style.width = "100%"
              el.classList.add("bg-success")
              el.classList.remove("progress-bar-animated","progress-bar-striped")
              if (undefined === active.children)
                active.children=[]
              active.children.push({module:file.name,leaf:true,parentNode:active})
              th.setState({active:active})

              }
            else{
              console.log(error);
            }
          })
    }
}

  // uploadLargeData(file){
  //   var size = file.size;
  //   var chunk_size = 4194304;
  //   var chunks = [];
  //   var offset = 0;
  //   var bytes = 0;
  //   var pubKey = this.state.pubKey
  //   var ethAcc = this.state.ethAcc
  //   const {active, activePath} = this.state
  //   var state = this.state
  //   readSlice(file);

  //   if (undefined === active.children)
  //       active.children=[]
  //   active.children.push({module:file.name,leaf:true,parentNode:active})
  //   this.setState({active:active})
    
  //   function closeFile(){
  //     console.log("Uploading the packet");
  //         Meteor.call("userShareDataFrontEnd",
  //         pubKey,
  //         ethAcc,
  //         activePath.replace("/data","")+'/'+file.name,
  //         "", 
  //         -1, 
  //         1,(error, result) => {
  //           if(!error){
  //             console.log('File Upload : success ' + 100 + '%');
  //             }
  //           else{
  //             console.log(error);
  //           }
  //         })
  //   }
  //   function readSlice(file){
  //     var reader = new FileReader();
  //     reader.onload = function(e){
  //     if (reader.readyState == FileReader.DONE){
  //       console.log("Uploading the packet");
  //         Meteor.call("userShareDataFrontEnd",
  //         pubKey,
  //         ethAcc,
  //         activePath.replace("/data","")+'/'+file.name,
  //         btoa(reader.result), 
  //         offset, 
  //         1,(error, result) => {
  //           if(!error){
  //             console.log('result ' + offset*100/size + '%');
  //             offset += chunk_size
  //             if(offset < size){
  //               readSlice(file)
  //             } else {
  //               closeFile();
  //             }
  //           } else{
  //             console.log('error ' + offset*100/size + '%');
  //             console.log(error);
  //           }
  //         })
  //       } else {
  //         console.log("Successful");
  //       }
  //     }
  //     var blob = file.slice(offset, offset + chunk_size);
  //     reader.readAsBinaryString(blob);
  //   }
  // }

  uploadData(event){
    const {active, activePath} = this.state
    if (active.leaf) {
      alert("Can't upload new data to existing file")
      return;
    }
    const filename = this.state.uploadFilename.name
    if (existChild(active, filename)){
      alert("Data file exist.");
      return;
    }


    if(this.state.uploadFilename.size > 4194304){
      this.uploadLargeData(this.state.uploadFilename);
    } else {
      var reader = new FileReader();
      reader.readAsBinaryString(this.state.uploadFilename);
      reader.onload = (fileLoadEvent) => {

        Meteor.call("userShareDataFrontEnd",
          this.state.pubKey,
          this.state.ethAcc,
          activePath.replace("/data","")+'/'+filename,
          btoa(reader.result), 
          -1, 
          0, (error, result) => {
            if(!error){
              console.log("Successful");
              if (undefined === active.children)
                active.children=[]
              active.children.push({module:filename,leaf:true,parentNode:active})
              this.setState({active:active})
            }
            else{
              console.log(error);
            }
          })
      }
    }
    this.handleCloseUploadData()
  }

  onGetDataStatistics(event){
    const {condition, active, activePath, activeType} = this.state
    if ("data" !== activeType)
      return
    this.getDataStatistics(condition, active, activePath)
  }

  getDataStatistics(condition, active, activePath) {
          //load data inspector
          Meteor.call("userLoadSingleDataInfo", activePath.replace("/data",""),
            condition, this.state.pubKey, this.state.ethAcc,
            (error, result) => {
              if (error) {
                if ('object' === typeof(error))
                  error = JSON.stringify(error)
                alert("Error:"+ error)
              }
              else if (result.error)
                console.log("Error:"+ result.error)
              else 
                active.content = result.result
                //if (active === this.state.active)
                this.setState({ active: active});
          })
  }

  onClickNode = node => {
    var activeType="other"
    //if (node.leaf) {
      var tmpNode = node
      var rootChild = node 
      //var currentPath=""
      var path=""
      while (undefined !== tmpNode.parentNode) {
        //path = currentPath+path
        path = "/"+tmpNode.module+path
        rootChild = tmpNode
        //currentPath = "/"+rootChild.module
        tmpNode = tmpNode.parentNode
      }
      if ("data" === rootChild.module)
        activeType = "data"
      else if ("model" === rootChild.module)
        activeType = "code"
    //}
    if (undefined !== node.parentNode)
      node.parentNode.collapsed = false
    if (node.leaf) {
      if ("code" === activeType) {
        this.setState({active: node, activeType:activeType,
          activePath:path, modelResult:node.module+" outputs",
          });
        if (!node.modified) {
          console.log("Path ---->::"+path)
          Meteor.call("userLoadModel", this.state.pubKey, 
           this.state.ethAcc, path.replace("/model",""), (error, result) => {
            if (error) {
              console.log("error:", error)
              if ("object" === typeof error)
                error = JSON.stringify(error)
              alert("Error:"+ error)
            }
            else if (result.error)
              alert("Error:"+ result.error)
            else {
              console.log("model downloaded");
              node.content = result.result
              this.setState({ active: node, activeType:activeType });
            }
          })
        } //else
          //this.setState({activeType:activeType});
        return;
      } else if ("data" === activeType) {
        if (undefined === node.content) {
          this.getDataStatistics(this.state.condition, node, path) 
        }
      }
    }
    if ("other" === activeType) {
        if (true === node.unloaded){
          //this.setState({active: node,activePath:path, activeType:activeType });
          this.refreshNode(node.parentNode, path.slice(0,-2), activeType)
        }
    }
    this.setState({ active: node, activeType:activeType,activePath:path });
  };

  renderNode = node => {
    var modified_tag=""
    if (node.modified)
      modified_tag=" *"
    return (
      <span
        className={cx('node', {
          'is-active': node === this.state.active
        })}
        onClick={this.onClickNode.bind(null, node)}
        onMouseDown={function(e){e.stopPropagation()}}
      >
        {node.module + modified_tag}
      </span>
    );
  };

  selectEnclave(event) {
    var pubKey = event.target.value;
    this.setState({ pubKey: pubKey })
    if ("Select" !== pubKey)
    Meteor.call("getEnclaveInfo", pubKey, this.state.ethAcc, "/", (err, res)=>{
      if (err)
        alert(err.toString())
      else if (res.error)
        alert(res.error)
      else {
        //var tree = renderTree(this.state.enclaves, res.result)
        renderTree(res.result);
        this.setState({tree:res.result, active:null, activeType:"other"})
      }
   })
  }

  selectEthAcc(event) {
    var ethAcc = event.target.value;
    this.setState({ ethAcc: ethAcc })
    if ("Select" !== ethAcc)
    Meteor.call("getEnclaveInfo", this.state.pubKey, ethAcc, (err, res)=>{
      if (err)
        alert(err.toString())
      else if (res.error)
        alert(res.error)
      else {
        //var tree = renderTree(this.state.enclaves, res.result)
        renderTree(res.result);
        this.setState({tree:res.result, active:null, activeType:"other"})
      }
   })
  }

  getCfg = () => {
    Meteor.call("userGetCfg", (err, result) => {
      if (!err) {
        if (result !== null) {
          var state = this.state;
          if (undefined !== result.enclaves)
            state.enclaves = result.enclaves;
            if (state.enclaves.length > 0)
              state.pubKey = state.enclaves[0].pubKey;
          state.ethAccounts = 
            result.blockchainAccounts.map((acc) => (acc.account))
          state.ethAcc = result.defaultEthAccount;
          this.setState(state)
          Meteor.call("getEnclaveInfo", state.pubKey, state.ethAcc,
          (err, res)=>{
            if (err)
              alert(err.toString())
            else if (res.error)
              alert(res.error)
            else {
              //var tree = renderTree(this.state.enclaves, res.result)
              renderTree(res.result);
              this.setState({tree:res.result, active:null, activeType:"other"})
            }
         })
        }
      }
    })
  }

  componentDidMount() {
    this.getCfg();
  }

  componentWillUnmount(){
  }

  handleChange = tree => {
    this.setState({
      tree: tree
    });
  };

  handleCloseUploadData() {
    this.setState({ showUploadData: false });
  }

  handleShowUploadData() {
    this.setState({ showUploadData: true });
  }

  handleCloseNewModel() {
    this.setState({ showNewModel: false });
  }

  handleShowNewModel() {
    this.setState({ showNewModel: true });
  }

  newModelModal = () => (
          <Modal
            show={this.state.showNewModel}
            onHide={this.handleCloseNewModel}
          >
            <Modal.Header closeButton>
              <Modal.Title>Enter the name of the new model</Modal.Title>
            </Modal.Header>
            <Modal.Body>
               <Form>
                 <DappFormGroup
                   id="newModelName"
                   label="New Model Name"
                   value={this.state.newModelName}
                   type="text"
                   onChange={event => {
                    var fn = event.target.value
                    this.setState({ newModelName: fn })
                   }}
                   placeholder="Name of the new model"
                 />
               </Form>
            </Modal.Body>
            <Modal.Footer>
               <Button variant="success"
                 onClick={this.newModelFile}>
                 Create a New Model
               </Button>
            </Modal.Footer>
          </Modal>
    )

  uploadDataModal = () => (
          <Modal
            show={this.state.showUploadData}
            onHide={this.handleCloseUploadData}
          >
            <Modal.Header closeButton>
              <Modal.Title>Choose a Data File to Upload</Modal.Title>
            </Modal.Header>
            <Modal.Body>
               <Form>
                 <DappFormGroup
                   id="dataUpload"
                   label="Select Files"
                   type="file"
                   onChange={event => {
                    var fn = event.target.files[0]
                    this.setState({ uploadFilename: fn })
                   }}
                 />
               </Form>
            </Modal.Body>
            <Modal.Footer>
               <Button variant="success"
                 onClick={()=>this.uploadData}>
                 Upload Data
               </Button>
            </Modal.Footer>
          </Modal>
    )

  render() {
    const { panes, enclaves, error, ethAccounts, active}
      = this.state;
    let child = <span>But I actually render here!</span>;

    const usrId = Meteor.userId();
    if (undefined === usrId || null === usrId) {
      return <Redirect push to='/data/login' />
    }

    var listActiveChildren = ""
    if (null !==  active)
      if (undefined !== active.children)
        listActiveChildren = active.children.map((child) => (
          <Col key={child.module} sm={3} 
             className={cx('node',{'is-active': child === this.state.active })}
             onClick={this.onClickNode.bind(null, child)} >
            {(child.leaf === true) &&
             <img src={"/document.png"} alt={"doc"} height={"48"} width={"48"}/> }
            {(child.leaf !== true) &&
             <img src={"/folder.png"} alt={"folder"} height={"48"} width={"48"}/> }
             <p>{child.module}</p>
          </Col> ))

    const listEthAcc = ethAccounts.map((acc) => (
      <option value={acc} key={acc.toString()}>{acc}</option>));

    const options = {
      lineNumbers: true,                     
      mode: { name: "text/x-rsrc" },         
      lineWrapping: true,
    };

    const showTree =()=>(
      <div className="tree">
      <Tree
        paddingLeft={20}
        tree={this.state.tree}
        onChange={this.handleChange}
        renderNode={this.renderNode}
      />
      </div>
    )

/*
                         onBlur={()=>{return}}
*/

    const codeEditorPane = () => {
      if (null === active)
        return (<div></div>)
      return(
                       <CodeMirror
                         ref="editor-code"
                         value={(null !== active) && active.content}
                         onBeforeChange={(editor, data, code) => {
                           var active = this.state.active
                           active.content = code
                           active.modified = true
                           this.setState({active: active });
                         }}
                         onChange={(editor, data, code) => {
                           var active = this.state.active
                           active.content = code
                           this.setState({active: active });
                         }}
                         options={options}
                         autofocus={true}
                         autoCursor={true}
                         autoScroll={true}
                         style={{ border: '1px', solid: '#ccc' }}
                       />
      )
    }

    const dataInspectorPane =() =>{
      const active = this.state.active
      if (null === active)
        return (<div></div>)
      //filterable
      return(
          <ReactTable
              data={active.content}

              defaultFilterMethod={
                (filter, row, column) => {
                  const id = filter.pivotId || filter.id
                  return row[id] !== undefined ? String(row[id]).includes(filter.value) : true
              }}

              getTdProps={(state, rowInfo, column, instance) => {
                return {
                  onClick: (e, handleOriginal) => {
                    if (handleOriginal) 
                      handleOriginal();
                  }
                }
              }}

              columns={[
                {
                  Header: "Data Inspector",
                  columns: [
/*
                    {
                      Header: "Data Name",
                      accessor: "fileName",
                      PivotValue: ({ value }) =>
                        <span style={{ color: "darkred" }}>
                          {value}
                        </span>
                    },
*/
                    {
                      Header: "Attribute",
                      accessor: "attributesName",
                      PivotValue: ({ value }) =>
                        <span style={{ color: "darkblue" }}>
                          {value}
                        </span>
                    },
                  ]
                },
                {
                  Header: "Data Statistical Info",
                  columns: [
                    {
                      Header: "Value",
                      accessor: "value",
                      sortable: false,
                    },
                    {
                      Header: "Distribution",
                      accessor: "ratio",
                      sortable: false,
                    },
                  ]
                }
              ]}
              pivotBy={["fileName", "attributesName"]}
              defaultPageSize={5}
              className="-striped -highlight"
            />

    )}

    const filePane = (contentPane) => {
      const active=this.state.active
      if (undefined !== active && null !== active){
        if (!active.leaf) {
         return(
           <Container>
              <Row>{listActiveChildren}</Row>
           </Container> )
        } else if (active.leaf)
         return(contentPane())
      }
      return (<div></div>)
    }

    const otherPane = (
            <SplitPane split="horizontal" defaultSize={36} allowResize={false} >
              <div>
                {[
                   {icon:"update", text:"Refresh Current Node", onClick:this.refreshTreeNode},
                   {icon:"edit", text:"Rename", onClick:this.newFile},
                   {icon:"trash", text:"Delete", onClick:() => this.deleteData}
                  ].map(btn=>(
                   <OverlayTrigger
                     key={btn.text}
                     placement="right"
                     overlay={
                       <Tooltip id="tooltip"
                       >
                         {btn.text}.
                       </Tooltip>
                     }
                   >
                     <img className="ide_button"
                       onClick = {btn.onClick}
                       src={"/"+btn.icon+".svg"} alt={btn.text}
                       width="24" height="24"/>
                   </OverlayTrigger>
                ))}
              </div>
              <Container> 
                <Row>{listActiveChildren}</Row>
              </Container>
            </SplitPane>
    )
              
    const dataPane = (
            <SplitPane split="horizontal" defaultSize={36} allowResize={false}
               pane2Style={{overflow:'auto'}}
            >
              <div>
                {[
                   {icon:"upload", text:"Upload Data", 
                     onClick:this.handleShowUploadData
                   },
                   {icon:"edit", text:"Rename", onClick:this.newFile},
                   {icon:"trash", text:"Delete", onClick:() => this.deleteData}
                  ].map(btn=>(
                   <OverlayTrigger
                     key={btn.text}
                     placement="right"
                     overlay={
                       <Tooltip id="tooltip-right">
                         {btn.text}.
                       </Tooltip>
                     }
                   >
                     <img className="ide_button"
                       onClick = {btn.onClick}
                       src={"/"+btn.icon+".svg"} alt={btn.text}
                       width="24" height="24"/>
                   </OverlayTrigger>
                ))}
                <span style={{marginLeft:"2em"}}>Conditions:</span>
                <input type="text" value={this.state.condition} name="condition"
                   onChange={(e)=>this.setState({condition:e.target.value})}
                   style={{height:'36px'}}
                />
                   <OverlayTrigger
                     key={"Get Data Statistics"}
                     placement="left"
                     overlay={
                       <Tooltip id="tooltip-left">
                         {"Get Data Statistics"}.
                       </Tooltip>
                     }
                   >
                     <img className="ide_button"
                       onClick = {this.onGetDataStatistics}
                       src={"/search.svg"} alt={"Get Stats"}
                       width="24" height="24"/>
                   </OverlayTrigger>
                {this.uploadDataModal()}
              </div>
              {filePane(dataInspectorPane)}
            </SplitPane>
    )

    const codePane = (
              <SplitPane split="horizontal" defaultSize={250} >
                {filePane(codeEditorPane)}
                <SplitPane split="horizontal" defaultSize={36} allowResize={false} >
                <div>
                  {[
                   {icon:"welcome-add-page", text:"New", onClick:this.handleShowNewModel},
                   {icon:"upload", text:"Save", onClick:this.uploadModel},
                   {icon:"controls-play", text:"Run", onClick:() => this.runModel()},
                   {icon:"controls-pause", text:"Pause", onClick:this.newFile},
                   {icon:"arrow-right-alt", text:"Step", onClick:this.newFile},
                   {icon:"dismiss", text:"Stop", onClick:this.newFile},
                   {icon:"trash", text:"Delete", onClick:() => this.deleteModel()}
                  ].map(btn=>(
                   <OverlayTrigger
                     key={btn.text}
                     placement="right"
                     overlay={
                       <Tooltip id="tooltip-right">
                         {btn.text}.
                       </Tooltip>
                     }
                   >
                     <img className="ide_button"
                       onClick = {btn.onClick}
                       src={"/"+btn.icon+".svg"} alt={btn.text}
                       width="24" height="24"/>
                   </OverlayTrigger>
                 ))}
                 {this.newModelModal()}
                </div>
                <textarea value={this.state.modelResult} readOnly 
                  style={{width:"100%", height:"100%",
                    color:"black", fontSize:"12px",
                    fontFamily:"Arial"
                  }}/>
                </SplitPane>
              </SplitPane>
    )
/*
                <Form>
                  <Form.Group controlId="formControlsTextarea">
                    <Form.Label><br />Result</Form.Label>
                    <Form.Control readOnly
                       value={this.state.modelResult}
                       as="textarea" placeholder="Result..."  />
                  </Form.Group>
                </Form>
*/
    return (
      <Container>
        <Row>
          <Col>
            <legend className="border border-top-0 border-left-0 border-right-0">Enclave Browser</legend>
          </Col>
        </Row>
        <Row>
        <Form>
          <Form.Row>
           <Form.Group as={Col} controlId="enclaveAcc">
             <Form.Label>Enclave</Form.Label>
             <Form.Control as="select"
               value={this.state.pubKey}
               onChange={this.selectEnclave}
             >
               <option defaultValue="">Select</option>
               {listEnclaves(enclaves)}
             </Form.Control>
           </Form.Group>
           <Form.Group as={Col} controlId="ethAcc">
             <Form.Label>Account</Form.Label>
             <Form.Control as="select"
               value={this.state.ethAcc}
               onChange={this.selectEthAcc}
             >
               <option defaultValue="">Select</option>
               {listEthAcc}
             </Form.Control>
           </Form.Group>
          </Form.Row>
        </Form>
        </Row>
        <Row 
          style={{border:'1px solid grey', padding:'0px',
            backgroundColor:'whitesmoke'}}
        >
            <SplitPane split="vertical" minSize={50}
              style={{ height:'400px',
                position:'relative', resize:'both', overflow:'scroll'
              }}
              defaultSize={200}
            >
              {showTree() }
              { (this.state.activeType === "other") && otherPane }
              { (this.state.activeType === "data") && dataPane }
              { (this.state.activeType === "code") && codePane }
            </SplitPane>
        </Row>
        <br/>
          <ProgressBar hidden id="fileProgressBar" animated />
      </Container>
    )
  }
}
/*
        <div 
          style={{border:'2px solid', padding:'20px', resize:'both',
            overflow:'auto'}}
        >
          <p>This is a test </p>
        </div>
          style={{padding: '0px', backgroundColor: 'whitesmoke',
          height: '480px', border: '1px solid grey', resize:'both' }}
            <SplitPane split="vertical" minSize={50}
              style={{position:'relative'}}
              defaultSize={200}
            >
              {showTree() }
              { (this.state.activeType === "other") && otherPane }
              { (this.state.activeType === "data") && dataPane }
              { (this.state.activeType === "code") && codePane }
            </SplitPane>
*/

export default withRouter(BrowseEnclave);
