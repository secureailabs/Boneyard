import {Meteor} from "meteor/meteor";

Meteor.methods({
    federated(nodes, master, donorAcc) {
        var Future = Npm.require('fibers/future');
        var result1 = new Future();
        var result2 = new Future();
        var result3 = new Future();
        var final_result = new Future();


        Meteor.call("userRunModelFile",
            nodes[0].pubKey, nodes[0].trainer, nodes[0].model, (error, result) => {
                if (error) {
                    console.log("error:", error);
                    result1.return(null)
                } else if (result.error) {
                    console.log("error:", result.error);
                    result1.return(null)
                }
                 else{
                    console.log("Run model returned.");
                    Meteor.call("userLoadFullFile","output.csv",nodes[0].pubKey,donorAcc,(error1,res)=> {
                        if (error) {
                            console.log("error:", error1);
                            result1.return(null)
                        } else if (res.error) {
                            console.log("error:", res.error);
                            result1.return(null)
                        } else {
                            result1.return(res.result)
                        }
                    })
                }
            });

      if (nodes.length > 1) {
        Meteor.call("userRunModelFile",
          nodes[1].pubKey, nodes[1].trainer, nodes[1].model, (error, result) => {
            if (error) {
              console.log("error:", error);
              result2.return(null)
            } else if (result.error) {
              console.log("error:", result.error);
              result2.return(null)
            }
            else
            {
                console.log("Run model returned.");
                Meteor.call("userLoadFullFile","output.csv",nodes[1].pubKey,donorAcc,(error1,res)=> {
                    if (error) {
                        console.log("error:", error1);
                        result2.return(null)
                    } else if (res.error) {
                        console.log("error:", res.error);
                        result2.return(null)
                    } else {
                      console.log("Full file result :: ")
                      console.log(res)
                        result2.return(res.result)
                    }
                })
            }
          })

      }


      if (nodes.length > 2) {
        Meteor.call("userRunModelFile",
          nodes[2].pubKey, nodes[2].trainer, nodes[2].model, (error, result) => {
            if (error) {
              console.log("error:", error);
              result3.return(null)
            } else if (result.error) {
              console.log("error:", result.error);
              result3.return(null)
            }
            else
            {
                console.log("Run model returned.");
                Meteor.call("userLoadFullFile","output.csv",nodes[2].pubKey,donorAcc,(error1,res)=> {
                    if (error) {
                        console.log("error:", error1);
                        result1.return(null)
                    } else if (res.error) {
                        console.log("error:", res.error);
                        result1.return(null)
                    } else {
                      console.log("Full file result :: ")
                      console.log(res)
                        result1.return(res.result)
                    }
                })
            }
          })

        }

      let output2 = undefined;
      let output3 = undefined;
        let output1 = result1.wait();
        if (nodes.length > 1)
          output2 = result2.wait();
        if (nodes.length > 2)
          output3 = result3.wait();


        console.log(output1);
        console.log(output2);
        console.log(output3);

        let tempOut1 = new Buffer(output1)
        let tempOut2 = new Buffer(output2)
        Meteor.call("userShareDataFrontEndE2E",
            master.pubKey, donorAcc, "output1.csv", tempOut1.toString('base64'), (error1, res1) => {
                if (error1) {
                    console.log("error:", error1);
                    final_result.return(null)
                } else if (res1.error) {
                    console.log("error:", res1.error);
                    final_result.return(null)
                }
                else
                {
                    console.log("First output uploaded to master.");
                    Meteor.call("userShareDataFrontEndE2E",
                      master.pubKey, donorAcc, "output2.csv", tempOut2.toString('base64'), (error2, res2) => {
                            if (error2) {
                                console.log("error:", error2);
                                final_result.return(null)
                            } else if (res2.error) {
                                console.log("error:", res2.error);
                                final_result.return(null)
                            }
                            else
                            {
                                console.log("Second output uploaded to master.");
                                // Meteor.call("userShareDataFrontEnd",
                                //   master.pubKey, master.trainer, "output3.txt", output3, (error3, res3) => {
                                //         if (error3) {
                                //             console.log("error:", error3);
                                //             final_result.return(null)
                                //         } else if (res3.error) {
                                //             console.log("error:", res3.error);
                                //             final_result.return(null)
                                //         }
                                //         else
                                //         {
                                //             console.log("Third output uploaded to master.");
                                //             Meteor.call("userRunModelFile",
                                //               master.pubKey, master.trainer, master.model, (error4, res4) => {
                                //                     if (error4) {
                                //                         console.log("error:", error4);
                                //                         final_result.return(null)
                                //                     } else if (res4.error) {
                                //                         console.log("error:", res4.error);
                                //                         final_result.return(null)
                                //                     }
                                //                     else{
                                //                         console.log("Master run successful");
                                //                         console.log(res4.result);
                                //                         final_result.return(res4.result)
                                //                     }
                                //                 })
                                //         }
                                //     })

                                Meteor.call("userRunModelFile",
                                              master.pubKey, master.trainer,"model.py", (error4, res4) => {
                                                    if (error4) {
                                                        console.log("error:", error4);
                                                        final_result.return(null)
                                                    } else if (res4.error) {
                                                        console.log("error:", res4.error);
                                                        final_result.return(null)
                                                    }
                                                    else{
                                                        console.log("Master run successful");
                                                        console.log(res4.result);
                                                        final_result.return(res4.result)
                                                    }
                                                })
                            }
                        })
                }
            });

        return final_result.wait()

    }
});