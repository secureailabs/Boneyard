import * as mocha from 'mocha';
import * as chai from 'chai';
import chaiHttp = require('chai-http');

import app from '../src/ApiApp';

chai.use(chaiHttp);
const expect: Chai.ExpectStatic = chai.expect;

describe('POST api/v1/aws/s3', () => {
  it('should be json', () => {
    return chai.request(app).post('/api/v1/aws/s3')
      .then(res => {
        expect(res.type).to.eql('application/json');
      });
  });

  it('should have a message prop', () => {
    return chai.request(app).post('/api/v1/aws/s3')
      .then(res => {
        expect(res.body.message).to.eql('POST /s3');
      });
  });
});
